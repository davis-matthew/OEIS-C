/*
 * A187173  Number of 3-step left-handed knight's tours (moves only out two,
 *          left one) on an n X n board summed over all starting positions.
 * https://oeis.org/A187173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*n*n - 64*n + 80 for n>3; a(n) = 0 for n <= 3
 * Range:     u8 n <= 7, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= (idx_t)3)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)12 * nn * nn;
    term += (seq_t)80;
    term -= (seq_t)64 * nn;
    return term;
}
