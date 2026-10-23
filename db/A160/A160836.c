/*
 * A160836  G.f.: (1+62*x+565*x^2+1050*x^3+485*x^4+52*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n*(n+1)*(277*n^4 + 629*n^3 + 1031*n^2 + 679*n +
 *            444)/90
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 1347,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute using 128‑bit intermediate to avoid overflow on 64‑bit builds. */
    __int128 nn  = ( __int128 ) n;
    __int128 n01 = nn + 1;
    __int128 n2  = nn * nn;       /* n^2   */
    __int128 n3  = n2 * nn;       /* n^3   */
    __int128 n4  = n3 * nn;       /* n^4   */
    __int128 poly = 277 * n4
                  + 629 * n3
                  + 1031 * n2
                  + 679 * nn
                  + 444;
    __int128 num  = (nn * n01) * poly;   /* n*(n+1)*poly   */
    __int128 val  = num / 90 + 1;        /* final value     */
    return (seq_t) val;
}
