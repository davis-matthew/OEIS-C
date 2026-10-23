/*
 * A047565  Numbers that are congruent to {0, 1, 3, 4, 5, 6, 7} mod 8.
 * https://oeis.org/A047565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-7) - a(n-8) with initial values a(1)=0,
 *            a(2)=1, a(3)=3, a(4)=4, a(5)=5, a(6)=6, a(7)=7. (Equivalently,
 *            a(n)=8*floor((n-1)/7) + ((n-1)%7 + ((n-1)%7>=2?1:0)))
 * Range:     u8 n <= 224, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behavior below offset is irrelevant */

    idx_t k = (n - 1) / 7u;         /* number of full blocks of 7 */
    idx_t t = (n - 1) % 7u;         /* position inside the current block */

    /* offset equals t except we add 1 for t >= 2 to skip the missing value */
    seq_t offset = (seq_t)t + (t >= 2u ? 1 : 0);

    seq_t val = (seq_t)k * 8 + offset;
    return val;
}
