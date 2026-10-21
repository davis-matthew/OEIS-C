/*
 * A200755  G.f. satisfies A(x) = 1 + x*A(x)^3 - x^2*A(x)^2.
 * https://oeis.org/A200755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} (-1)^k * binomial(3*n-4*k, k) *
 *            binomial(3*n-5*k, n-2*k) / (2*n-3*k+1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper to compute binomial coefficients using 128‑bit arithmetic.
   It is used only for intermediate values; the final result is cast
   back to seq_t. */
static __int128 binom128(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (__int128)(n - i + 1) / (__int128)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return (seq_t)1;

    /* Sum over k, using signed 128‑bit accumulator to handle
       intermediate cancellations. */
    __int128 sum = 0;

    idx_t maxk = n / 2;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        __int128 t1 = binom128(3*n - 4*k, k);
        __int128 t2 = binom128(3*n - 5*k, n - 2*k);
        __int128 term = t1 * t2;
        term = term / (__int128)(2*n - 3*k + 1);

        if (k & 1)
            sum -= term;
        else
            sum += term;
    }

    return (seq_t)sum;
}
