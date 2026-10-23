/*
 * A040539  Continued fraction for sqrt(563).
 * https://oeis.org/A040539
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=23; for n≥1, a(n)=2^{[2|n]}·23^{[5|n]}, where [P] is 1 if
 *            predicate P is true, otherwise 0
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;

    seq_t ans = (seq_t)1;

    if ((n & 1) == 0)                 /* divisible by 2 */
        ans *= (seq_t)2;

    if (n % 5 == 0)                   /* divisible by 5 */
        ans *= (seq_t)23;

    return ans;
}
