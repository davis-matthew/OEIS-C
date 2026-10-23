/*
 * A183510  Number of n X 2 binary arrays with each element equal to either
 *          the sum mod 2 of its horizontal and vertical neighbors or the sum
 *          mod 2 of its diagonal and antidiagonal neighbors.
 * https://oeis.org/A183510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 27, u64 n <= 54, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (empirical recurrence):
   a(1)=2, a(2)=4, a(3)=11, a(4)=24, a(5)=56, a(6)=121, a(7)=272
   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-4) + a(n-5) - a(n-6) - 6*a(n-7)
   for n ≥ 8
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                 /* undefined for n=0 */
    static const seq_t init[7] = {
        (seq_t)2,
        (seq_t)4,
        (seq_t)11,
        (seq_t)24,
        (seq_t)56,
        (seq_t)121,
        (seq_t)272
    };

    if (n <= 7) return init[n - 1];

    /* sliding window for the 7 most recent terms */
    seq_t a1 = init[0], a2 = init[1], a3 = init[2];
    seq_t a4 = init[3], a5 = init[4], a6 = init[5];
    seq_t a7 = init[6];
    seq_t val = a7;   /* placeholder */

    for (idx_t i = 8; i <= n; ++i) {
        val = (seq_t)2 * a7 + a6 - (seq_t)2 * a4 + a3 - a2 - (seq_t)6 * a1;
        /* shift window: drop a1, move others forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = a7;
        a7 = val;
    }
    return val;
}
