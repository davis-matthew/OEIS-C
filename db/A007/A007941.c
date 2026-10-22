/*
 * A007941  Some nontrivial permutation of digits is a cube.
 * https://oeis.org/A007941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: n (d digits) is a term iff some cube c < 10^d, padded
 *            with leading zeros to d digits, has the same digit multiset as
 *            n, and either c != n, or c == n and n has a repeated digit (a
 *            nontrivial permutation then fixes n)
 * Range:     u8 n <= 12, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   digit_sig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sig.h"

static uint64_t *cube_val = NULL, *cube_sig = NULL;
static int *cube_len = NULL;
static long long ncubes = 0, cube_cap = 0;

static uint64_t *terms = NULL;
static long long nterms = 0, terms_cap = 0;
static uint64_t next_cand = 1;

static void ensure_cubes(uint64_t limit) {
    /* make sure all cubes < limit are present */
    while (1) {
        uint64_t k = (uint64_t)ncubes;
        uint64_t c = k * k * k;
        if (ncubes > 0 && cube_val[ncubes - 1] >= limit) return;
        if (c >= limit && ncubes > 0) {
            /* keep one cube beyond limit as sentinel */
        }
        if (ncubes == cube_cap) {
            long long nc = cube_cap ? cube_cap * 2 : 256;
            cube_val = (uint64_t *)realloc(cube_val, (size_t)nc * sizeof(uint64_t));
            cube_sig = (uint64_t *)realloc(cube_sig, (size_t)nc * sizeof(uint64_t));
            cube_len = (int *)realloc(cube_len, (size_t)nc * sizeof(int));
            cube_cap = nc;
        }
        int l;
        cube_val[ncubes] = c;
        cube_sig[ncubes] = digit_sig(c, &l);
        cube_len[ncubes] = l;
        ncubes++;
    }
}

static bool is_term(uint64_t m) {
    int d;
    uint64_t s = digit_sig(m, &d);
    if (d < 2) return false;
    bool rep = false;
    for (int i = 0; i < 10; i++)
        if (((s >> (5 * i)) & 31) >= 2) rep = true;
    uint64_t lim = 1;
    for (int i = 0; i < d; i++) lim *= 10;
    ensure_cubes(lim);
    for (long long k = 0; k < ncubes && cube_val[k] < lim; k++) {
        uint64_t ps = cube_sig[k] + (uint64_t)(d - cube_len[k]);
        if (ps == s) {
            if (cube_val[k] != m || rep) return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    while ((idx_t)nterms < n) {
        uint64_t m = next_cand++;
        if (is_term(m)) {
            if (nterms == terms_cap) {
                long long nc = terms_cap ? terms_cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, (size_t)nc * sizeof(uint64_t));
                terms_cap = nc;
            }
            terms[nterms++] = m;
        }
    }
    return (seq_t)terms[n - 1];
}
