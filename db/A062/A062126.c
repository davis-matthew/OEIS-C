/*
 * A062126  Sixth column of A046741.
 * https://oeis.org/A062126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (320 + 1114*n + 1515*n^2 + 1125*n^3 + 405*n^4 +
 *            81*n^5)/40
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 72, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic to avoid overflow for all n that fit in the test set */
    unsigned __int128 nn = n;          /* n as 128‑bit */
    unsigned __int128 t = 320;         /* running numerator */

    t += 1114 * nn;                    /* 1114*n */

    unsigned __int128 n2 = nn * nn;    /* n^2 */
    t += 1515 * n2;                    /* 1515*n^2 */

    unsigned __int128 n3 = n2 * nn;    /* n^3 */
    t += 1125 * n3;                    /* 1125*n^3 */

    unsigned __int128 n4 = n3 * nn;    /* n^4 */
    t += 405 * n4;                     /* 405*n^4 */

    unsigned __int128 n5 = n4 * nn;    /* n^5 */
    t += 81 * n5;                      /* 81*n^5 */

    t /= 40;                           /* final division */

    return (seq_t)t;
}
