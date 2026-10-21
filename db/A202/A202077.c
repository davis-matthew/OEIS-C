/*
 * A202077  Number of arrays of 5 integers in -n..n with sum zero and the sum
 *          of every adjacent pair being odd.
 * https://oeis.org/A202077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (230*n^4 + 552*n^3 + 424*n^2 + 96*n) / 384 for n even
 *            a(n) = (230*n^4 + 368*n^3 + 148*n^2 + 16*n + 6) / 384 for n odd
 * Range:     u8 none, u16 n <= 3, u32 n <= 67, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the (unused) offset 0 case */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;        /* n */
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n3 = n2 * nn;         /* n^3 */
    seq_t n4 = n2 * n2;         /* n^4 */
    seq_t num;

    if ((n & 1) == 0) {         /* even n */
        num = ((seq_t)230) * n4
            + ((seq_t)552) * n3
            + ((seq_t)424) * n2
            + ((seq_t)96) * nn;
    } else {                    /* odd n */
        num = ((seq_t)230) * n4
            + ((seq_t)368) * n3
            + ((seq_t)148) * n2
            + ((seq_t)16) * nn
            + 6;
    }
    return num / 384;           /* division is exact */
}
