/*
 * A101269  a(1)=0, a(2)=1, a(n+2) = (8*n^2+2*n+1)*a(n+1) -
 *          2*n*(2*n-1)^3*a(n).
 * https://oeis.org/A101269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, a(2) = 1; a(n+2) = (8*n^2 + 2*n + 1)*a(n+1) -
 *            2*n*(2*n-1)^3*a(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    seq_t a_prev2 = (seq_t)0; /* a(1) */
    seq_t a_prev1 = (seq_t)1; /* a(2) */

    for (idx_t k = 1; k <= n - 2; ++k)
    {
        /* term1 = 8*k^2 + 2*k + 1 */
        seq_t term1 = (seq_t)8 * k * k + (seq_t)2 * k + 1;

        /* term2 = 2*k*(2*k-1)^3 */
        seq_t two_k      = (seq_t)2 * k;
        seq_t two_k_m1   = two_k - 1;                 /* 2*k-1   */
        seq_t t3         = two_k_m1 * two_k_m1 * two_k_m1; /* (2*k-1)^3 */
        seq_t term2      = (seq_t)2 * k * t3;          /* 2*k*(2*k-1)^3 */

        seq_t a_next = term1 * a_prev1 - term2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_next;
    }

    return a_prev1;
}
