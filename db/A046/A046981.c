/*
 * A046981  Denominators of Taylor series for exp(x)*cos(x).
 * https://oeis.org/A046981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = n! / 2^[n/2] for n ≢ 2 (mod 4); a(4k+2) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A046981.
   Uses only integer arithmetic, works for arbitrary-precision seq_t. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Quick return for the special case n ≡ 2 (mod 4) */
    if ((n & 3u) == 2u)            /* n % 4 == 2 */
        return (seq_t)1;

    /* Compute n! / 2^(n/2) by multiplying and cancelling 2s as we go */
    seq_t val = (seq_t)1;
    idx_t twos_needed = n / 2;     /* number of divisions by 2 required */

    for (idx_t k = 1; k <= n; ++k) {
        val *= (seq_t)k;
        /* Remove a factor of 2 from val as long as we still need them */
        while ((val & (seq_t)1) == 0 && twos_needed > 0) {
            val >>= 1;             /* equivalent to val /= 2 */
            --twos_needed;
        }
    }

    /* In principle twos_needed should be zero here.
       If any remain (due to pathological large numbers), divide them out. */
    if (twos_needed > 0)
        val >>= twos_needed;

    return val;
}
