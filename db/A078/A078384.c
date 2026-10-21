/*
 * A078384  Sum of all prime factors of numbers which can be written as sum of
 *          a positive square and a positive cube.
 * https://oeis.org/A078384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001414(A055394(n)) * A055394(n) = n-th integer that can
 *            be written as x^2 + y^3 (x,y>=1) * A001414(m) = sum of all prime
 *            factors of m (with multiplicity)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, int_cube_root, sum_pf_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_spf.h"
#include "../utils/int_cube_root.h"
#include "../utils/sum_pf_value.h"

/* comparison routine for qsort on seq_t values */
static int cmp_seq_t(const void *p1, const void *p2)
{
    seq_t v1 = *(const seq_t *)p1;
    seq_t v2 = *(const seq_t *)p2;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* generate all values <= lim that are representable as x^2 + y^3,
 * store unique sorted values in *sums and the number of terms in *terms */
static void generate_terms(idx_t lim, size_t *terms, seq_t **sums)
{
    /* dynamic list for candidate values */
    size_t cap = 256;
    size_t len = 0;
    seq_t *cand = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cand) exit(1);

    idx_t max_y = int_cube_root(lim);          /* floor(cbrt(lim)) */
    for (idx_t y = 1; y <= max_y; ++y) {
        seq_t y3 = (seq_t)y * (seq_t)y * (seq_t)y;
        for (idx_t x = 1;; ++x) {
            seq_t val = y3 + (seq_t)x * (seq_t)x;
            if (val > lim) break;
            if (len == cap) {
                cap <<= 1;
                cand = (seq_t *)realloc(cand, cap * sizeof(seq_t));
                if (!cand) exit(1);
            }
            cand[len++] = val;
        }
    }

    /* sort and deduplicate */
    qsort(cand, len, sizeof(seq_t), cmp_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (uniq == 0 || cand[i] != cand[uniq - 1]) {
            cand[uniq++] = cand[i];
        }
    }

    /* build SPF table up to lim */
    idx_t *spf = NULL;
    build_spf(lim, &spf);

    /* compute sums of prime factors for each unique value */
    seq_t *arr = (seq_t *)malloc(uniq * sizeof(seq_t));
    if (!arr) exit(1);
    for (size_t i = 0; i < uniq; ++i) {
        arr[i] = sum_pf_value(cand[i], spf);
    }

    /* clean up temporary data */
    free(cand);
    free(spf);

    *terms = uniq;
    *sums  = arr;
}

/* public sequence function */
seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *sum_arr = NULL;
    static size_t term_count = 0;
    static idx_t current_limit = 0;

    if (!initialized) {
        current_limit = 3000000;   /* initial limit: enough for >1e4 terms */
        generate_terms(current_limit, &term_count, &sum_arr);
        initialized = true;
    }

    size_t idx = (size_t)(n - 1);
    while (idx >= term_count) {   /* need more terms: double the limit */
        current_limit <<= 1;
        free(sum_arr);
        generate_terms(current_limit, &term_count, &sum_arr);
    }

    return sum_arr[idx];
}
