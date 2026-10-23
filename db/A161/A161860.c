/*
 * A161860  Squares which are anagrams of cubes.
 * https://oeis.org/A161860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = square numbers that are anagrams of some cube number
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 1234, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_counts, floor_cube_root, pack_counts, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"       /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/floor_cube_root.h" /* idx_t floor_cube_root(seq_t x); */
#include "../utils/digit_counts.h"    /* void digit_counts(seq_t x, unsigned char cnt[10]); */
#include "../utils/pack_counts.h"

/* Compare two uint64_t values for qsort/bsearch. */
static int cmp_uint64(const void *a, const void *b) {
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Dynamic containers for cube keys, indexed by digit count d. */
static size_t digit_arrays_cap = 0;
static size_t *cube_size  = NULL;   /* number of keys stored for digit d */
static size_t *cube_cap   = NULL;   /* allocated capacity for digit d */
static bool   *cube_computed = NULL;/* computed flag for digit d */
static uint64_t **cube_keys = NULL; /* sorted key arrays for digit d */

/* Ensure storage for all ints up to index d, doubling as needed. */
static void expand_digit_arrays_if_needed(size_t d) {
    if (d < digit_arrays_cap) return;
    size_t new_cap = digit_arrays_cap ? digit_arrays_cap * 2 : 16;
    while (new_cap <= d) new_cap <<= 1;

    cube_size   = (size_t *)realloc(cube_size,   new_cap * sizeof(size_t));
    cube_cap    = (size_t *)realloc(cube_cap,    new_cap * sizeof(size_t));
    cube_computed = (bool *)realloc(cube_computed, new_cap * sizeof(bool));
    cube_keys   = (uint64_t **)realloc(cube_keys, new_cap * sizeof(uint64_t *));
    for (size_t i = digit_arrays_cap; i < new_cap; ++i) {
        cube_size[i] = 0;
        cube_cap[i]  = 0;
        cube_computed[i] = false;
        cube_keys[i] = NULL;
    }
    digit_arrays_cap = new_cap;
}

/* Generate all cube keys having exactly d decimal digits. */
static void ensure_cube_for_digit(size_t d) {
    expand_digit_arrays_if_needed(d);
    if (cube_computed[d]) return;

    /* Lower and upper bounds for cubes with d digits */
    seq_t lower = pow_uint(10, d - 1);       /* 10^(d-1)   */
    seq_t upper = pow_uint(10, d);           /* 10^d       */

    /* Find the first and last cube roots that give d‑digit cubes. */
    idx_t root_min = (idx_t)floor_cube_root(lower - (seq_t)1) + 1;
    idx_t root_max = (idx_t)floor_cube_root(upper - (seq_t)1);

    if (root_min > root_max) {   /* no cubes with this digit count */
        cube_computed[d] = true;
        return;
    }

    size_t num = (size_t)(root_max - root_min + 1);
    cube_keys[d] = (uint64_t *)malloc(num * sizeof(uint64_t));
    cube_size[d] = num;
    cube_cap[d]  = num;

    unsigned char cnt[10];
    size_t pos = 0;
    for (idx_t r = root_min; r <= root_max; ++r) {
        seq_t cube = pow_uint((uint64_t)r, 3);
        digit_counts(cube, cnt);
        cube_keys[d][pos++] = pack_counts(cnt);
    }
    qsort(cube_keys[d], cube_size[d], sizeof(uint64_t), cmp_uint64);
    cube_computed[d] = true;
}

/* Return the number of decimal digits of a seq_t value. */
static int seq_digit_len(seq_t x) {
    int len = 0;
    seq_t t = x;
    if (t == 0) return 1;
    while (t > (seq_t)0) {
        t /= (seq_t)10;
        ++len;
    }
    return len;
}

/* Persistent data used by successive calls. */
static seq_t *terms = NULL;          /* stored squares that match cubes */
static size_t terms_cap = 0;         /* allocated capacity of terms array */
static size_t terms_len = 0;         /* number of terms currently stored */

static idx_t next_root = 1;          /* next square root to examine */

/* Main function required by the problem. */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for 0, but return 0 as a placeholder */

    idx_t target = n - 1; /* 0‑based index */

    /* Ensure we have computed enough terms to answer the request. */
    if (terms_len <= target) {
        if (terms_cap == 0) {
            terms_cap = 128;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        }

        while (terms_len <= target) {
            seq_t square = pow_uint((uint64_t)next_root, 2);

            int d = seq_digit_len(square);
            if ((size_t)d >= digit_arrays_cap) expand_digit_arrays_if_needed(d);
            ensure_cube_for_digit(d);

            unsigned char cnt[10];
            digit_counts(square, cnt);
            uint64_t key = pack_counts(cnt);

            bool found = false;
            if (cube_computed[d]) {
                found = (bsearch(&key, cube_keys[d], cube_size[d], sizeof(uint64_t), cmp_uint64) != NULL);
            }

            if (found) {
                if (terms_len == terms_cap) {
                    terms_cap *= 2;
                    terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                }
                terms[terms_len++] = square;
            }
            ++next_root;
        }
    }

    return terms[target];
}
