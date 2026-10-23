/*
 * A130759  Partial sums of A130707.
 * https://oeis.org/A130759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^n A130707(k) * A130707(0) = 1 * For n ≥ 1:
 *            A130707(n) = 2·A130707(n−1) + d_n * where the period‑6 pattern
 *            of d_n is [0, −2, −2, 0, 2, 2]. * a(n) is the partial sum of
 *            A130707 up to n
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state to allow out‑of‑order calls */
    static seq_t *sum = NULL;          /* sum array: a(i) */
    static size_t sum_cap = 0;         /* allocated size of sum */
    static idx_t max_n = 0;            /* highest n computed so far */
    static seq_t prevA = 0;            /* A(n) for the highest n computed */

    /* pattern of adjustments d_n for n ≥ 1 */
    static const int d_pattern[6] = {0, -2, -2, 0, 2, 2};

    /* initialise on first call */
    if (sum == NULL) {
        sum_cap = 1;
        sum = (seq_t *)malloc(sum_cap * sizeof(seq_t));
        if (!sum) return 0; /* memory allocation failure, return 0 */
        sum[0] = (seq_t)1;  /* a(0) = 1, also A(0) = 1 */
        prevA = (seq_t)1;
        max_n = 0;
    }

    /* extend precomputation up to requested n */
    if ((idx_t)max_n < n) {
        size_t new_cap = (size_t)(n + 1);
        /* reallocate sum array */
        seq_t *tmp = (seq_t *)realloc(sum, new_cap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        sum = tmp;
        sum_cap = new_cap;

        /* compute new terms sequentially */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* d_i uses the pattern, indexing with (i-1) modulo 6 */
            int d = d_pattern[(int)((i - 1) % 6)];
            seq_t nextA = prevA << 1;          /* 2*prevA */
            if (d > 0) {
                nextA += (seq_t)d;
            } else if (d < 0) {
                nextA -= (seq_t)(-d);
            }   /* else d == 0 -> no change */

            /* accumulate partial sum */
            sum[i] = sum[i - 1] + nextA;

            /* update state for next iteration */
            prevA = nextA;
            max_n = i;
        }
    }

    /* return the requested partial sum */
    return sum[n];
}
