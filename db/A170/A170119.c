/*
 * A170119  Number of reduced words of length n in Coxeter group on 14
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12 * Σ_{k=1}^{37} a(n-k) – 78 * a(n-38) + c_n where c_0 =
 *            1, c_1 … c_37 = 2, c_38 = 1, and c_n = 0 for n > 38
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer holding all previously computed terms */
    static seq_t *buf   = NULL;   /* buf[i] = a(i) */
    static idx_t  size  = 0;      /* number of terms currently stored */

    /* enlarge buffer if needed */
    if (n >= size) {
        size_t new_cap = size ? size * 2 : 1;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        buf = tmp;

        /* compute terms from old size up to n */
        for (idx_t i = size; i <= n; ++i) {
            /* RHS coefficient from the numerator polynomial */
            seq_t rhs;
            if (i == 0)          rhs = (seq_t)1;
            else if (i <= 38)    rhs = (i == 38) ? (seq_t)1 : (seq_t)2;
            else                 rhs = (seq_t)0;

            /* sum of the previous 37 terms (if they exist) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= 37; ++k) {
                if (i >= k) sum += buf[i - k];
            }

            /* recurrence: a(n) = 12*sum – 78*a(n-38) + rhs   (a(n-38)=0 for n<38) */
            seq_t term = rhs + (seq_t)12 * sum;
            if (i >= 38) term -= (seq_t)78 * buf[i - 38];

            buf[i] = term;
        }
        size = n + 1;
    }
    return buf[n];
}
