/*
 * A160769  G.f.: (21 + 101*x + 97*x^2 + 22*x^3 + x^4)/(1 - x)^5.
 * https://oeis.org/A160769
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 121*n^4/12 + 40*n^3 + 869*n^2/12 + 125*n/2 + 21
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 142, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 128‑bit intermediate to avoid overflow when seq_t is 64‑bit.
       The final cast produces the desired seq_t value.  */
    __int128 nn = n;
    __int128 n2 = nn * nn;      /* n^2   */
    __int128 n3 = n2 * nn;      /* n^3   */
    __int128 n4 = n3 * nn;      /* n^4   */
    __int128 num = 121 * n4
                  + 480 * n3
                  + 869 * n2
                  + 750 * nn
                  + 252;
    return (seq_t)(num / 12);
}
