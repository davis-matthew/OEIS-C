/*
 * A167099  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 44·∑_{k=1}^{12} a(n-k) – 990·a(n-13) + r_n where r_0 = 1,
 *            r_1 … r_12 = 2, r_13 = 1, r_n = 0 for n ≥ 14. This recurrence
 *            follows from the rational generating function given in the OEIS
 *            entry
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;        /* cached values */
    static size_t cap = 0;           /* allocated capacity */
    static size_t len = 0;           /* number of terms stored */

    size_t target = (size_t)n + 1;   /* we need values up to a(n) */

    /* grow the buffer if necessary */
    if (cap < target) {
        size_t new_cap = cap ? cap : 16;
        while (new_cap < target) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        buf = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    for (size_t i = len; i < target; ++i) {
        seq_t total = 0;

        /* 44·∑_{k=1}^{12} a(n-k) */
        for (size_t k = 1; k <= 12 && i >= k; ++k) {
            total += (seq_t)44 * buf[i - k];
        }

        /* – 990·a(n-13) */
        if (i >= 13) {
            total -= (seq_t)990 * buf[i - 13];
        }

        /* add the non‑homogeneous term r_i */
        seq_t r;
        if (i == 0)         r = 1;
        else if (i <= 12)   r = 2;
        else if (i == 13)   r = 1;
        else                r = 0;

        buf[i] = total + r;
    }

    len = target;
    return buf[n];
}
