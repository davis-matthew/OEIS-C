/*
 * A165381  Number of slanted n X 4 (i=1..n) X (j=i..4+i-1) 1..4 arrays with
 *          all 1s connected, all 2s connected, all 3s connected, all 4s
 *          connected, 1 in the upper left corner, 2 in the upper right
 *          corner, 3 in the lower left corner, 4 in the lower right corner,
 *          and with no element having more than 3 neighbo...
 * https://oeis.org/A165381
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*(100*n^2 - 540*n + 817) for n >= 6 Initial values for n
 *            = 2,3,4,5: a(2) = 37 a(3) = 285 a(4) = 1012 a(5) = 2467 The
 *            sequence is defined for n >= 2
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases defined by the OEIS entry. */
    if (n <= 5) {
        switch (n) {
            case 2: return 37;
            case 3: return 285;
            case 4: return 1012;
            case 5: return 2467;
            default: return 0;   /* n < 2: undefined, return 0 */
        }
    }

    /* Compute a(n) = 4*(100*n^2 - 540*n + 817) for n >= 6. */
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)100 * nn * nn
                - (seq_t)540 * nn
                + (seq_t)817;
    return (seq_t)4 * val;
}
