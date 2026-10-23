/*
 * A156675  a(n) = 17*((100^(n+1) - 1)/99).
 * https://oeis.org/A156675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*((100^(n+1)-1)/99) Equivalent recurrence: a(0)=17,
 *            a(1)=1717, a(n)=101*a(n-1)-100*a(n-2)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 17;
    }
    if (n == 1) {
        return 1717;
    }

    seq_t prev2 = 17;
    seq_t prev1 = 1717;
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        curr = 101 * prev1 - 100 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return curr;
}
