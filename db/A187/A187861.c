/*
 * A187861  Number of 6-step one space for components leftwards or up, two
 *          space for components rightwards or down asymmetric quasi-queen's
 *          tours (antidiagonal moves become knight moves) on an n X n board
 *          summed over all starting positions.
 * https://oeis.org/A187861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28294*n^2 - 224508*n + 433614 for n>9; For n<=9 the
 *            values are given explicitly
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 9) {
        switch ((idx_t)n) {
            case 1: return 0;
            case 2: return 0;
            case 3: return 846;
            case 4: return 9932;
            case 5: return 47962;
            case 6: return 126397;
            case 7: return 262409;
            case 8: return 452766;
            case 9: return 707541;
        }
    }
    seq_t tn = (seq_t)n;
    return (seq_t)28294 * tn * tn - (seq_t)224508 * tn + (seq_t)433614;
}
