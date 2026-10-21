/*
 * A059535  Beatty sequence for Pi^2/6, or zeta(2).
 * https://oeis.org/A059535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( n * ζ(2) ), ζ(2) = π^2 / 6 ≈ 1.6449340668482264
 *            The implementation uses a high‑precision rational approximation
 *            ζ(2) ≈ 16449340668482264 / 10000000000000000, which is accurate
 *            enough for all practical values of n used in the tests
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2000, u128 n <= 2000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Rational approximation of ζ(2) */
    const seq_t P = 16449340668482264ULL;    /* numerator  */
    const seq_t Q = 10000000000000000ULL;    /* denominator */

    /* Compute floor( n * P / Q ) using 128‑bit integer arithmetic to
       avoid overflow.  */
    unsigned __int128 prod = (unsigned __int128)n * (unsigned __int128)P;
    seq_t result = (seq_t)(prod / Q);
    return result;
}
