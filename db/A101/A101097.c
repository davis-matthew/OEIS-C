/*
 * A101097  a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(2 + 4*n + n^2)/840.
 * https://oeis.org/A101097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(n+4)*(2 + 4*n + n^2)/840
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 60, u64 n <= 1477,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use a 128‑bit intermediate to avoid overflow on 64‑bit builds */
    unsigned __int128 nn = n;                 /* convert to 128‑bit */
    unsigned __int128 term = 
        nn * (nn + 1) * (nn + 2) * (nn + 3) * (nn + 4) *
        ((nn * nn) + 4 * nn + 2);              /* n^2 + 4n + 2 */
    term /= 840;                               /* exact division */

    return (seq_t)term;                       /* cast back to seq_t */
}
