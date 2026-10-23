/*
 * A040528  Continued fraction for sqrt(552).
 * https://oeis.org/A040528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 23, a(n) = 2 if n is odd (n≥1), a(n) = 46 if n is even
 *            (n≥2)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This follows the continued fraction expansion of sqrt(552) which is
   [23; 2,46,2,46,2,46, ...] */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;
    /* For n ≥ 1, the sequence alternates between 2 and 46.    */
    return (n & 1) ? (seq_t)2 : (seq_t)46;
}
