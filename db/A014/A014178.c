/*
 * A014178  a(n) = Sum_{k = 0..n} binomial(n,k)^3*binomial(n+k,k).
 * https://oeis.org/A014178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k = 0..n} binomial(n,k)^3*binomial(n+k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes a(n) for OEIS A014178 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t result   = 1; /* term for k = 0 */
    seq_t c_nk     = 1; /* binomial(n,0) */
    seq_t c_np_k   = 1; /* binomial(n+0,0) */

    for (idx_t k = 1; k <= n; ++k) {
        /* C(n,k) = C(n,k-1) * (n-k+1) / k */
        c_nk = c_nk * (seq_t)(n - k + 1) / (seq_t)k;
        /* C(n+k,k) = C(n+k-1,k-1) * (n+k) / k */
        c_np_k = c_np_k * (seq_t)(n + k) / (seq_t)k;
        /* term = C(n,k)^3 * C(n+k,k) */
        seq_t term = c_nk * c_nk * c_nk * c_np_k;
        result += term;
    }

    return result;
}
