/*
 * A105947  a(n) = C(n+4,4) * C(n+6,6).
 * https://oeis.org/A105947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binom(n+4,4) * binom(n+6,6)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 21, u64 n <= 220, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h> /* for uint64_t, may be used by seq_t/idx_t */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* Compute C(n+4,4) = (n+4)(n+3)(n+2)(n+1)/24 */
    seq_t c4 = (N + 4) * (N + 3) * (N + 2) * (N + 1) / 24;

    /* Compute C(n+6,6) = (n+6)(n+5)(n+4)(n+3)(n+2)(n+1)/720 */
    seq_t c6 = (N + 6) * (N + 5) * (N + 4) * (N + 3) * (N + 2) * (N + 1) / 720;

    /* Return the product */
    return c4 * c6;
}
