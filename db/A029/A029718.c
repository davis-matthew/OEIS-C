/*
 * A029718  Numbers of form 2x^2 + 2xy + 3y^2.
 * https://oeis.org/A029718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A029718:
   numbers of the form 2*x^2 + 2*x*y + 3*y^2.
   The sequence is sorted in ascending order and indices start at 1. */

#include <stdlib.h>
#include <stdint.h>

/* integer square root (floor) for a 64‑bit unsigned value */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* comparator for qsort on seq_t */
static int cmp_seq(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* cache of computed sequence values */
static seq_t *seq_cache   = NULL;   /* pointer to sorted unique values */
static size_t seq_count   = 0;      /* number of values in cache   */
static seq_t seq_max_val  = 0;      /* largest bound used so far   */

/* generate all representable numbers <= bound, store sorted unique values */
static void generate_upto(seq_t bound) {
    /* compute bounds for y and x */
    idx_t y_square_limit;
    {
        __uint128_t tmp = (__uint128_t)bound * 2;
        y_square_limit = (idx_t)(tmp / 5);          /* floor((2*bound)/5)  */
    }
    idx_t y_max = isqrt_u64(y_square_limit);          /* maximal |y| with Q_min <= bound */

    idx_t x_limit = (idx_t)(((__uint128_t)bound) / 2);/* since 2*x^2 <= bound   */
    idx_t x_max   = isqrt_u64(x_limit);              /* maximal |x| needed   */

    size_t cap = (size_t)(2 * x_max + 1) * (size_t)(2 * y_max + 1) + 1;
    seq_t *temp = (seq_t *)malloc(cap * sizeof(seq_t));
    size_t sz   = 0;

    for (long long yi = -(long long)y_max; yi <= (long long)y_max; ++yi) {
        for (long long xi = -(long long)x_max; xi <= (long long)x_max; ++xi) {
            long long val = 2LL * xi * xi + 2LL * xi * yi + 3LL * yi * yi;
            if (val < 0) continue;
            if ((seq_t)val <= bound) {
                temp[sz++] = (seq_t)val;
            }
        }
    }

    /* sort and deduplicate */
    qsort(temp, sz, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < sz; ++i) {
        if (i == 0 || temp[i] != temp[i-1]) {
            temp[uniq++] = temp[i];
        }
    }

    /* replace cache */
    if (seq_cache) free(seq_cache);
    seq_cache      = temp;
    seq_count      = uniq;
    seq_max_val    = bound;
}

/* public sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* invalid index for offset 1, harmless */

    seq_t bound = seq_max_val;
    if (bound == 0) bound = 1;      /* initial bound */

    /* grow bound until we have at least n values */
    while (1) {
        if (!seq_cache || bound > seq_max_val) {
            generate_upto(bound);
        }
        if (seq_count >= (size_t)n) break;
        if (bound > ((seq_t)-(size_t)1) / 2) break; /* guard against overflow */
        bound *= 2;
    }

    return seq_cache[n - 1];
}
