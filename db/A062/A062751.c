/*
 * A062751  Coefficient array for certain polynomials N(4; k,x) (rising powers
 *          in x).
 * https://oeis.org/A062751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 13, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A062751: Coefficient array for certain polynomials N(4; k,x) (rising powers in x).
 * Recurrence:
 *   a(n) = a(n-1) + …  (as described in the OEIS entry).
 * The algorithm incrementally builds the coefficient vectors for N(4; k,x)
 * until it has stored at least n+1 terms in a flat buffer.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/binomial.h"          /* provides binomial(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *buf   = NULL;          /* flat array of all terms */
    static size_t buf_cap = 0;           /* capacity of buf */
    static size_t buf_len = 0;           /* number of terms already stored */

    static seq_t *prev = NULL;           /* previous level coefficient vector */
    static size_t prev_len = 0;          /* length of previous vector */
    static idx_t m = 0;                  /* current level (0‑based) */

    /* initialise on first call */
    if (buf == NULL) {
        buf_cap = 128;
        buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
        if (!buf) return 0;            /* out of memory – hard failure */

        /* level 0: N(4;0,x)=1 -> coefficient vector {1} */
        prev = (seq_t *)malloc(sizeof(seq_t));
        if (!prev) { free(buf); return 0; }
        prev[0] = 1;
        prev_len = 1;

        buf[0] = 1;
        buf_len = 1;

        m = 1;                          /* next level to compute */
    }

    /* extend the buffer until we have at least n+1 terms */
    while (buf_len <= n) {
        /* current level: m */
        size_t cur_len = 3 * m + 1;
        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));
        if (!cur) return 0;                   /* out of memory */

        /* pre‑compute terms that are independent of k */
        seq_t bin_4m_m = binomial(4 * m + 1, m);
        seq_t denom = (seq_t)(4 * m + 1);

        for (size_t k = 0; k < cur_len; ++k) {
            /* term = C(3*m+1, k+1) * C(4*m+1, m) / (4*m+1) */
            seq_t term = binomial(3 * m + 1, k + 1);
            term = term * bin_4m_m;
            term = term / denom;

            /* apply sign (-1)^k */
            if (k & 1U) term = -term;

            if (k <= 3 * m - 4U && k + 1 < prev_len) {
                /* use previous level's coefficient a(m‑1, k+1) */
                cur[k] = prev[k + 1] + term;
            } else {
                cur[k] = term;
            }
        }

        /* append current level to the flat buffer */
        if (buf_len + cur_len > buf_cap) {
            size_t new_cap = buf_cap * 2;
            if (new_cap < buf_len + cur_len) new_cap = buf_len + cur_len;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) { free(cur); return 0; }
            buf = tmp;
            buf_cap = new_cap;
        }
        for (size_t i = 0; i < cur_len; ++i) {
            buf[buf_len + i] = cur[i];
        }
        buf_len += cur_len;

        /* free previous level and promote current level */
        free(prev);
        prev = cur;
        prev_len = cur_len;
        ++m;                                   /* next level */
    }

    return buf[n];
}
