/*
 * A165312  a(0)=1, a(1)=5, a(n)=11*a(n-1)-25*a(n-2) for n>1.
 * https://oeis.org/A165312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(n) = 11*a(n-1) - 25*a(n-2) for n > 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)5;
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)11 * prev1 - (seq_t)25 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
