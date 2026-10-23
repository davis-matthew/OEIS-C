/*
 * A041180  Numerators of continued fraction convergents to sqrt(101).
 * https://oeis.org/A041180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0)=10, a(1)=201; a(n)=20*a(n-1)+a(n-2) for n>1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return 10;
    if (n == 1u) return 201;
    seq_t prev2 = 10;
    seq_t prev1 = 201;
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        cur = 20 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
