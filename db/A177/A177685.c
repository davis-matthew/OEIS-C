/*
 * A177685  a(n) = 6*a(n-1)-8*a(n-2) for n > 4; a(0)=603, a(1)=4731,
 *          a(2)=58834, a(3)=254204, a(4)=1032696.
 * https://oeis.org/A177685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1)-8*a(n-2) for n>4; a(0)=603, a(1)=4731,
 *            a(2)=58834, a(3)=254204, a(4)=1032696
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t init[5] = { 603, 4731, 58834, 254204, 1032696 };

seq_t a(idx_t n)
{
    if (n < 5)
        return init[n];

    seq_t prev2 = init[3];   /* a(3) */
    seq_t prev1 = init[4];   /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t cur = (seq_t)6 * prev1 - (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
