/*
 * A040436  Continued fraction for sqrt(458).
 * https://oeis.org/A040436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A040436: Continued fraction for sqrt(458)
    a(0) = 21,
    a(n) = 42 when (n-1) mod 3 = 2, otherwise 2  (for n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)21;

    /* Compute the remainder of (n-1) divided by 3 */
    idx_t r = (n - 1) % 3;
    return r == 2 ? (seq_t)42 : (seq_t)2;
}
