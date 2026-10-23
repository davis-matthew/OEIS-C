/*
 * A121681  a(n) = A121680(n)/(n+1) = [x^n] (1 + x*(1+x)^(n+1) )^(n+1) /
 *          (n+1).
 * https://oeis.org/A121681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n+1} C(n+1,k) * C((n+1)*k, n-k) / (n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t N = (seq_t)n + 1;            /* N = n + 1 */
    seq_t C_n1_k = 1;                  /* C(N,0) */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0) {
            /* C(N,k) = C(N,k-1) * (N-k+1) / k   (division is exact) */
            C_n1_k = C_n1_k * (N - k + 1) / k;
        }

        /* binom((n+1)*k , n-k)  – top value fits in idx_t */
        idx_t top    = (idx_t)(N * k);
        idx_t bottom = (idx_t)(n - k);
        seq_t C_Nk_nk = binom(top, bottom);

        sum += C_n1_k * C_Nk_nk;
    }

    sum /= N;   /* divide by (n+1) – the result is always integer */
    return sum;
}
