/*
 * A156872  Period 12: 1,3,-1,3,1,0,-1,-3,1,-3,-1,0 repeated.
 * https://oeis.org/A156872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = periodic with period 12: a(0)=1, a(1)=3, a(2)=-1, a(3)=3,
 *            a(4)=1, a(5)=0, a(6)=-1, a(7)=-3, a(8)=1, a(9)=-3, a(10)=-1,
 *            a(11)=0, and a(n)=a(n+12) for all n≥0
 * Range:     i8 n <= 59, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Periodic array of 12 values */
    static const seq_t periodic[12] = {
        1,  3, -1, 3, 1, 0,
        -1, -3, 1, -3, -1, 0
    };
    idx_t r = n % 12ULL;
    return periodic[r];
}
