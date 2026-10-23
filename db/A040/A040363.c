/*
 * A040363  Continued fraction for sqrt(383).
 * https://oeis.org/A040363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the partial quotients in the continued fraction
   expansion of sqrt(383).  The digits are given by
   a(0) = 19, and for n ≥ 1,
   a(n) = p[(n-1) mod 8]   with   p = [1, 1, 3, 19, 3, 1, 1, 38].
*/
seq_t a(idx_t n)
{
    /* Special case for the first term of the expansion */
    if (n == 0)
        return 19;          /* a(0) */

    /* Periodic part of the continued fraction */
    static const seq_t block[8] = { 1, 1, 3, 19, 3, 1, 1, 38 };

    /* (n-1) is safe since n > 0 here; modulo 8 yields an index in 0..7 */
    idx_t idx = (n - 1) % 8;
    return block[idx];
}
