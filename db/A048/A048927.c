/*
 * A048927  Numbers that are the sum of 5 positive cubes in exactly 2 ways.
 * https://oeis.org/A048927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7588, u32 n <= 8058, u64 n <= 8058,
 *            u128 n <= 8058, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A048927: numbers that are the sum of 5 positive cubes in exactly two ways. */
/* The value for a given index n (1‑based) is returned.  For index 0 or a value
   beyond the computed range 0 is returned. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* largest cube needed: floor(cuberoot(2243453)) = 130 */
#define MAX_CUBE     130
/* largest possible summed value */
#define MAX_VALUE   2243453

/* forward declarations */
static void init_sequence(void);

/* static storage for the computed terms */
static seq_t *terms = NULL;
static size_t term_count = 0;
static bool initialized = false;

/* Helper to grow a dynamic array of seq_t values */
static void grow_seq(seq_t **arr, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap * 2);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) return;         /* allocation failure ignored – caller will get 0 */
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[*len] = val;
    (*len)++;
}

/* Compute all terms once */
static void init_sequence(void)
{
    /* count array for all sums */
    unsigned short *cnt = (unsigned short *)calloc(MAX_VALUE + 1U, sizeof(unsigned short));
    if (!cnt) return; /* memory failure ignored for brevity */

    /* pre‑compute cubes in 64‑bit to avoid overflow before converting to seq_t */
    uint64_t cube[MAX_CUBE + 1U];
    for (idx_t i = 0; i <= (idx_t)MAX_CUBE; ++i)
        cube[i] = (uint64_t)i * (uint64_t)i * (uint64_t)i;

    /* enumerate unordered 5‑tuples a≤b≤c≤d≤e */
    for (idx_t a = 1; a <= (idx_t)MAX_CUBE; ++a) {
        uint64_t a3 = cube[a];
        if (a3 > (uint64_t)MAX_VALUE - 4U) break;          /* need at least 4 ones */
        for (idx_t b = a; b <= (idx_t)MAX_CUBE; ++b) {
            uint64_t sb = a3 + cube[b];
            if (sb > (uint64_t)MAX_VALUE - 3U) break;
            for (idx_t c = b; c <= (idx_t)MAX_CUBE; ++c) {
                uint64_t sc = sb + cube[c];
                if (sc > (uint64_t)MAX_VALUE - 2U) break;
                for (idx_t d = c; d <= (idx_t)MAX_CUBE; ++d) {
                    uint64_t sd = sc + cube[d];
                    if (sd > (uint64_t)MAX_VALUE - 1U) break;
                    for (idx_t e = d; e <= (idx_t)MAX_CUBE; ++e) {
                        uint64_t s = sd + cube[e];
                        if (s > (uint64_t)MAX_VALUE) break;
                        /* s fits into idx_t because MAX_VALUE < 2^32 */
                        cnt[(idx_t)s]++;
                    }
                }
            }
        }
    }

    /* collect terms that have exactly two representations and fit in seq_t */
    seq_t *tmp_terms = NULL;
    size_t tmp_len   = 0;
    size_t tmp_cap   = 0;

    for (idx_t val = 1; val <= (idx_t)MAX_VALUE; ++val) {
        if ((unsigned short)cnt[val] == 2) {
            seq_t t = (seq_t)val;
            /* keep only values that are representable without overflow */
            if ((idx_t)t == (idx_t)val) {
                grow_seq(&tmp_terms, &tmp_len, &tmp_cap, t);
            }
        }
    }

    free(cnt);

    terms      = tmp_terms;
    term_count = tmp_len;
    initialized = true;
}

/* exported function – requires external linkage */
seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0 || n > (idx_t)term_count) return (seq_t)0;
    return terms[n - 1];
}
