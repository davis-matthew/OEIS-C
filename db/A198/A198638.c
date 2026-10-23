/*
 * A198638  Number of 2n X 2 0..2 arrays with values 0..2 introduced in row
 *          major order and each element equal to an even number of horizontal
 *          and vertical neighbors.
 * https://oeis.org/A198638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=36, a(3)=376, a(n)=12*a(n-1)-16*a(n-2) for n>3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)36;
    if (n == 3) return (seq_t)376;

    seq_t prev2 = (seq_t)36;   /* a(2) */
    seq_t prev1 = (seq_t)376;  /* a(3) */
    seq_t val = 0;

    for (idx_t i = 4; i <= n; ++i) {
        val = (seq_t)12 * prev1 - (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = val;
    }
    return val;
}
