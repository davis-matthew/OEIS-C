/*
 * A108053  Maximum number of diagonals of a regular n-gon that meet at a
 *          non-center point.
 * https://oeis.org/A108053
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 (n≤4); for n>4: - a(n)=2 if n is odd or n==6 - a(n)=3
 *            if n is even and n%6≠0 - a(n)=4 if n==12 - a(n)=5 if n%6==0 but
 *            n%30≠0 - a(n)=7 if n%30==0 (This implements the OEIS description
 *            A108053)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 4ULL)              /* n = 3,4 ⇒ 0 */
        return (seq_t)0;

    if (n == 6ULL)              /* special case */
        return (seq_t)2;

    if ((n & 1ULL) == 1ULL)     /* odd n (except 6 handled above) */
        return (seq_t)2;

    if (n % 6ULL != 0ULL)       /* even but not divisible by 6 */
        return (seq_t)3;

    if (n == 12ULL)             /* n divisible by 6, special case */
        return (seq_t)4;

    if (n % 30ULL == 0ULL)       /* n divisible by 30 */
        return (seq_t)7;

    return (seq_t)5;              /* remaining multiples of 6 (not 12, not 30) */
}
