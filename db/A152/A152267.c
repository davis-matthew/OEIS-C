/*
 * A152267  a(n) = ((9 + sqrt(8))^n + (9 - sqrt(8))^n)/2.
 * https://oeis.org/A152267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 73*a(n-2), a(0) = 1, a(1) = 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)9;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = (seq_t)18 * prev1 - (seq_t)73 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
