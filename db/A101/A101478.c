/*
 * A101478  G.f. satisfies A(x) = x*(1+A(x))^4/(1+A(x)^2).
 * https://oeis.org/A101478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(0)=0, a(1)=1, a(2)=4, a(3)=21 a(n) =
 *            (12*n*(n-1)*(2*n-3)*a[n-1] -12*(n-1)*(16*n^2-64*n+65)*a[n-2]
 *            +16*(2*n-5)*(4*n-9)*(4*n-11)*a[n-3])/(n*(n-1)*(n+1)) (for n>=3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static idx_t cap = 0;
    static seq_t *val = NULL;

    if (n < cap)
        return val[n];

    /* grow cache to size n+1 */
    idx_t new_cap = n + 1;
    seq_t *new_val = (seq_t *)realloc(val, sizeof(seq_t) * new_cap);
    if (!new_val) abort();
    val = new_val;

    for (idx_t i = cap; i <= n; ++i) {
        if (i == 0) {
            val[i] = (seq_t)0;
        } else if (i == 1) {
            val[i] = (seq_t)1;
        } else if (i == 2) {
            val[i] = (seq_t)4;
        } else if (i == 3) {
            val[i] = (seq_t)21;
        } else {
            /* compute a(i) using the recurrence */
            if (sizeof(seq_t) <= 8) {
                /* use 128‑bit intermediate to avoid overflow */
                unsigned __int128 n128  = i;
                unsigned __int128 n1    = n128 - 1;
                unsigned __int128 nplus = n128 + 1;

                unsigned __int128 term1 = 12;
                term1 *= n128;
                term1 *= n1;
                term1 *= (2 * n128 - 3);
                term1 *= (unsigned __int128)val[i - 1];

                unsigned __int128 poly = 16 * n128 * n128 - 64 * n128 + 65;
                unsigned __int128 term2 = 12;
                term2 *= n1;
                term2 *= poly;
                term2 *= (unsigned __int128)val[i - 2];

                unsigned __int128 term3 = 16;
                term3 *= (2 * n128 - 5);
                term3 *= (4 * n128 - 9);
                term3 *= (4 * n128 - 11);
                term3 *= (unsigned __int128)val[i - 3];

                unsigned __int128 numerator = term1 + term3 - term2;
                unsigned __int128 denom     = n128 * n1 * nplus;
                val[i] = (seq_t)(numerator / denom);
            } else {
                /* arbitrary‑precision path – normal arithmetic */
                seq_t n_val = (seq_t)i;
                seq_t n1    = n_val - 1;

                seq_t term1 = 12;
                term1 *= n_val;
                term1 *= n1;
                term1 *= (seq_t)(2 * n_val - 3);
                term1 *= val[i - 1];

                seq_t poly = (seq_t)16 * n_val * n_val - (seq_t)64 * n_val + 65;
                seq_t term2 = 12;
                term2 *= n1;
                term2 *= poly;
                term2 *= val[i - 2];

                seq_t term3 = 16;
                term3 *= (seq_t)(2 * n_val - 5);
                term3 *= (seq_t)(4 * n_val - 9);
                term3 *= (seq_t)(4 * n_val - 11);
                term3 *= val[i - 3];

                seq_t numerator = term1 + term3 - term2;
                seq_t denom     = n_val * n1 * ((seq_t)n_val + 1);
                val[i] = numerator / denom;
            }
        }
    }

    cap = new_cap;
    return val[n];
}
