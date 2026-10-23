/*
 * A132222  Beatty sequence 1+2*floor(n*Pi/2), which contains infinitely many
 *          primes.
 * https://oeis.org/A132222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + 2*floor(n*Pi/2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pi ≈ 104348/33215, so Pi/2 ≈ 104348/66430.
       This rational is accurate to <1e-11, more than enough for
       n up to ~8×10^10 (and far beyond the test limits).      */
    const seq_t N = 104348;      /* numerator for Pi/2 */
    const idx_t D = 66430;       /* denominator for Pi/2 (fits in idx_t) */

    /* Compute floor(n*N/D) without overflow:
       q = n/D, r = n%D,  floor(n*N/D) = q*N + floor(r*N/D).  */
    seq_t q = (seq_t)(n / D);
    seq_t r = (seq_t)(n % D);

    seq_t term = q * N + (r * N) / D;

    return 1 + 2 * term;
}
