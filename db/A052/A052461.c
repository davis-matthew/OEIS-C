/*
 * A052461  4-magic series constant.
 * https://oeis.org/A052461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n^8 + 15*n^6 + 10*n^4 - 1)/30
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 165,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A052461 – 4‑magic series constant. */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for safety on 64‑bit builds */
    unsigned __int128 nn = n;          /* n as 128‑bit integer */
    unsigned __int128 n2 = nn * nn;    /* n^2 */
    unsigned __int128 n4 = n2 * n2;    /* n^4 */
    unsigned __int128 n6 = n4 * n2;    /* n^6 */
    unsigned __int128 n8 = n4 * n4;    /* n^8 */

    unsigned __int128 inner = (unsigned __int128)6 * n8
                            + (unsigned __int128)15 * n6
                            + (unsigned __int128)10 * n4
                            - 1;

    unsigned __int128 num = nn * inner;  /* n * inner  */
    unsigned __int128 res = num / 30;    /* divide by 30 */

    return (seq_t)res;                   /* cast to the return type */
}
