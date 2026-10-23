/*
 * A183897  Number of nondecreasing arrangements of n+3 numbers in 0..2 with
 *          each number being the sum mod 3 of three others.
 * https://oeis.org/A183897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + 9*n - 2)/2 for n > 2, a(1) = 1, a(2) = 7
 * Range:     u8 n <= 18, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the quadratic expression observed for A183897.  */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    /* Use 128‑bit arithmetic to avoid overflow for all usable values. */
    __uint128_t t   = ( __uint128_t ) n;
    __uint128_t val = t * t + t * ( __uint128_t )9 - ( __uint128_t )2;
    val /= ( __uint128_t )2;
    return (seq_t)val;
}
