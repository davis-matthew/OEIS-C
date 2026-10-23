/*
 * A021054  Expansion of 1/((1-x)(1-2x)(1-3x)(1-11x)).
 * https://oeis.org/A021054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 77*a(n-2) + 127*a(n-3) - 66*a(n-4); Initial
 *            values: a(0)=1, a(1)=17, a(2)=212, a(3)=2422
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 17;
    if (n == 2) return 212;
    if (n == 3) return 2422;

    seq_t prev4 = 1, prev3 = 17, prev2 = 212, prev1 = 2422;
    seq_t curr = 0;

    for (idx_t i = 4; i <= n; ++i) {
        curr = 17 * prev1 - 77 * prev2 + 127 * prev3 - 66 * prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
