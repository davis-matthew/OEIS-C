/*
 * A040357  Continued fraction for sqrt(377).
 * https://oeis.org/A040357
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

/* OEIS A040357 – continued fraction terms for sqrt(377)
   a(0)=19;
   for n>=1: a(n)=38 when (n-1)≡3 (mod 4), otherwise a(n)=2 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 19;                 /* integer part of sqrt(377) */
    /* period 4: 2,2,2,38 */
    idx_t r = (n - 1) & 3;          /* (n-1) % 4 using bitwise AND */
    return (r == 3) ? 38 : 2;
}
