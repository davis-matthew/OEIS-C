/*
 * A187300  Number of 5-step one space leftwards or up, two space rightwards
 *          or down asymmetric rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 214*n^2 - 1080*n + 1152 for n>7; first 10 terms
 *            hard‑coded
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 10) {
        static const seq_t tbl[11] = {
            0,   /* n=0 (unused) */
            0,   /* n=1 */
            0,   /* n=2 */
            90,  /* n=3 */
            416, /* n=4 */
            1208, /* n=5 */
            2400, /* n=6 */
            4092, /* n=7 */
            6208, /* n=8 */
            8766, /* n=9 */
            11752 /* n=10 */
        };
        return tbl[(int)n];
    }
    seq_t tn = (seq_t)n;
    return (seq_t)214 * tn * tn - (seq_t)1080 * tn + (seq_t)1152;
}
