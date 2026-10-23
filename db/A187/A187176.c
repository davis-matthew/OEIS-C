/*
 * A187176  Number of 6-step left-handed knight's tours (moves only out two,
 *          left one) on an n X n board summed over all starting positions.
 * https://oeis.org/A187176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 284*n^2 - 3100*n + 7944 for n>9; a(1..5)=0; a(6)=88;
 *            a(7)=496; a(8)=1440; a(9)=3064
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5) return 0;
    if (n == 6) return 88;
    if (n == 7) return 496;
    if (n == 8) return 1440;
    if (n == 9) return 3064;

    seq_t nn = (seq_t)n;
    seq_t val = 284 * nn * nn - 3100 * nn + 7944;
    return val;
}
