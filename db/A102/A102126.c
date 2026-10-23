/*
 * A102126  Minimum number of pieces needed to dissect a square into n smaller
 *          squares (not necessarily of the same size).
 * https://oeis.org/A102126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n for n > 5; a(1) = 1, a(2)=a(3)=a(4)=4, a(5)=6
 * Range:     u8 n <= 29, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 5) {
        switch ((int)n) {
            case 1: return 1;
            case 2: case 3: case 4: return 4;
            case 5: return 6;
        }
    }
    return (seq_t)n;
}
