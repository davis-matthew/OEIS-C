/*
 * A180593  Digital root of 3n.
 * https://oeis.org/A180593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital root of (3*n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* dr(0) = 0 */
    idx_t r = n % 3;                      /* 0,1,2 are the only possibilities */
    return (seq_t)(r == 0 ? 9 : r * 3);    /* 9 if n≡0 (mod3), 3 if n≡1, 6 if n≡2 */
}
