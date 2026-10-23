/*
 * A191363  Numbers m whose deficiency is 2: sigma(m) - 2*m = -2.
 * https://oeis.org/A191363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = (F(n-1)-1)*F(n-1)/2 where F(k)=2^(2^k)+1 (Fermat prime)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Known Fermat primes (F(k) for k = 0..4).  The value fits in seq_t. */
    static const seq_t fp[] = {
        (seq_t)3u,    /* F(0) = 2^(2^0)+1 = 3   */
        (seq_t)5u,    /* F(1) = 2^(2^1)+1 = 5   */
        (seq_t)17u,   /* F(2) = 2^(2^2)+1 = 17  */
        (seq_t)257u,  /* F(3) = 2^(2^3)+1 = 257 */
        (seq_t)65537u /* F(4) = 2^(2^4)+1 = 65537 */
    };
    const size_t fp_count = sizeof fp / sizeof fp[0];

    /* Offsets are 1‑based in OEIS: a(1) corresponds to F(0). */
    if (n == 0)          /* Out of bounds, guard against underflow. */
        return (seq_t)0;

    /* Convert to 0‑based index and check bounds. */
    idx_t idx = n - 1;
    if (idx >= fp_count)
        return (seq_t)0; /* No known Fermat prime for this n. */

    seq_t p = fp[idx];
    seq_t result = (p - (seq_t)1) * p / (seq_t)2;
    return result;
}
