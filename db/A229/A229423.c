/*
 * A229423  Number of n X 3 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, rows
 *          lexicographically nondecreasing, and columns lexicographically
 *          nonincreasing.
 * https://oeis.org/A229423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^6 + 18*n^5 + 100*n^4 + 240*n^3 + 799*n^2 + 642*n +
 *            1080) / 360
 * Range:     u8 none, u16 n <= 3, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          (OEIS A229423) */

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t n2 = s * s;          /* n^2 */
    seq_t n3 = n2 * s;         /* n^3 */
    seq_t n4 = n3 * s;         /* n^4 */
    seq_t n5 = n4 * s;         /* n^5 */
    seq_t n6 = n5 * s;         /* n^6 */

    seq_t num = n6 + (seq_t)18 * n5 + (seq_t)100 * n4
               + (seq_t)240 * n3 + (seq_t)799 * n2
               + (seq_t)642 * s + (seq_t)1080;

    return num / (seq_t)360;
}
