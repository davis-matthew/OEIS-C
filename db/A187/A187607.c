/*
 * A187607  Number of 3-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-bishop's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*n^2 - 80*n + 100 for n>3 a(1) = 0, a(2) = 0, a(3) = 9
 * Range:     u8 n <= 6, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 3) {
        switch ((int)n) {
            case 1: return 0;
            case 2: return 0;
            case 3: return 9;
        }
    }
    seq_t tn = (seq_t)n;
    return 16 * tn * tn - 80 * tn + 100;
}
