/*
 * A099195  a(n) = n^2*(n^6 + 28*n^4 + 154*n^2 + 132)/315.
 * https://oeis.org/A099195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 * (n^6 + 28*n^4 + 154*n^2 + 132) / 315
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 32, u64 n <= 525,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128–bit intermediate to avoid overflow before division */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 nsq = nn * nn;          /* n^2 */
    unsigned __int128 n3  = nsq * nn;         /* n^3 */
    unsigned __int128 n4  = n3 * nn;          /* n^4 */
    unsigned __int128 n6  = n3 * n3;          /* n^6 */

    unsigned __int128 term = n6 + (unsigned __int128)28 * n4
                           + (unsigned __int128)154 * nsq
                           + (unsigned __int128)132;

    unsigned __int128 num = nsq * term;      /* n^2 * term */

    /* 315 always divides num exactly */
    seq_t result = (seq_t)(num / 315U);
    return result;
}
