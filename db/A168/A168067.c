/*
 * A168067  a(n) = n*(n^7 + 1)/2.
 * https://oeis.org/A168067
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^7 + 1)/2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 279,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* Compute n^7 */
    seq_t p = sn;          /* n^1 */
    p *= sn;               /* n^2 */
    p *= sn;               /* n^3 */
    p *= sn;               /* n^4 */
    p *= sn;               /* n^5 */
    p *= sn;               /* n^6 */
    p *= sn;               /* n^7 */

    seq_t term = p + 1;    /* n^7 + 1 */

    /* Divide by 2 while keeping arithmetic exact for 64‑bit builds */
    if ((n & 1) == 0) {    /* n even: (n/2)*(n^7+1) */
        return (sn / 2) * term;
    } else {               /* n odd: n*((n^7+1)/2) */
        return sn * (term / 2);
    }
}
