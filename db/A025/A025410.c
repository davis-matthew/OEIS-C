/*
 * A025410  Numbers that are the sum of 4 distinct positive cubes in exactly 3
 *          ways.
 * https://oeis.org/A025410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A025410: Numbers that are the sum of 4 distinct positive cubes in exactly 3 ways.
   The implementation enumerates all 4‑cube combinations up to a cube limit,
   records the sums, counts how many times each sum occurs, and keeps only those
   sums that occur exactly three times. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* comparison function for qsort on seq_t */
static int seq_compare(const void *p1, const void *p2) {
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

/* storage for the computed sequence */
static seq_t *seq_vals = NULL;
static size_t   seq_len = 0;

/* generate the first TARGET terms lazily */
static void compute_sequence(void) {
    const size_t TARGET = 10000;          /* number of terms we need */
    size_t cube_limit = 60;               /* start value; increase if needed */

    while (true) {
        size_t N      = cube_limit;
        size_t combos = (N * (N - 1) * (N - 2) * (N - 3)) / 24;   /* C(N,4) */

        /* pre‑compute the first N positive cubes */
        seq_t *cubes = (seq_t *)malloc(N * sizeof(seq_t));
        for (size_t i = 0; i < N; ++i) {
            seq_t v = (seq_t)(i + 1);
            cubes[i] = v * v * v;
        }

        /* generate all sums of 4 distinct cubes */
        seq_t *sums = (seq_t *)malloc(combos * sizeof(seq_t));
        size_t idx = 0;
        for (size_t i = 0; i <= N - 4; ++i)
            for (size_t j = i + 1; j <= N - 3; ++j)
                for (size_t k = j + 1; k <= N - 2; ++k)
                    for (size_t l = k + 1; l <= N - 1; ++l) {
                        sums[idx++] = cubes[i] + cubes[j] + cubes[k] + cubes[l];
                    }

        /* sort the sums to enable frequency counting */
        qsort(sums, combos, sizeof(seq_t), seq_compare);

        /* collect sums that occur exactly 3 times */
        seq_t *results = (seq_t *)malloc(1024 * sizeof(seq_t));
        size_t res_cap = 1024;
        size_t res_len = 0;

        size_t p = 0;
        while (p < combos) {
            seq_t val = sums[p];
            size_t q = p + 1;
            while (q < combos && sums[q] == val) ++q;
            size_t cnt = q - p;
            if (cnt == 3) {
                if (res_len == res_cap) {
                    res_cap *= 2;
                    results = (seq_t *)realloc(results, res_cap * sizeof(seq_t));
                }
                results[res_len++] = val;
            }
            p = q;
        }

        free(cubes);
        free(sums);

        if (res_len >= TARGET) {          /* enough terms produced */
            seq_vals = results;
            seq_len  = res_len;
            break;
        }

        /* not enough terms – discard and try again with a larger limit */
        free(results);
        cube_limit += 20;                  /* increase the cube limit */
    }
}

seq_t a(idx_t n) {
    /* lazy initialization */
    static bool initialized = false;
    if (!initialized) {
        compute_sequence();
        initialized = true;
    }

    /* return the n‑th term (1‑based), or 0 if out of range */
    if (n == 0 || n > (idx_t)seq_len) return (seq_t)0;
    return seq_vals[(size_t)n - 1];
}
