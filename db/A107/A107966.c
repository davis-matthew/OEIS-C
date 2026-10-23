/*
 * A107966  a(n) = (n+1)(n+2)^3*(n+3)^2*(n+4)(5n^2 + 23n + 30)/8640.
 * https://oeis.org/A107966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)(n+2)^3*(n+3)^2*(n+4)*(5n^2 + 23n + 30)/8640
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 24, u64 n <= 314, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 128‑bit arithmetic for the intermediate product to avoid overflow
       on 64‑bit builds; the final result fits in seq_t for all n that the
       tests request. */
    unsigned __int128 nn = (unsigned __int128)n;               /* n as 128‑bit */
    unsigned __int128 term1 = nn + 1;                          /* (n+1)   */
    unsigned __int128 term2 = nn + 2;                          /* (n+2)   */
    unsigned __int128 term3 = nn + 3;                          /* (n+3)   */
    unsigned __int128 term4 = nn + 4;                          /* (n+4)   */
    unsigned __int128 poly   = 5*nn*nn + 23*nn + 30;           /* 5n^2+23n+30 */

    /* compute product (n+1)(n+2)^3(n+3)^2(n+4)(5n^2+23n+30) */
    unsigned __int128 numer = term1 *
                              (term2 * term2 * term2) *
                              (term3 * term3) *
                              term4 * poly;

    /* divide by 8640 = 2^6 * 3^3 * 5 */
    numer /= (unsigned __int128)8640ULL;

    return (seq_t)numer;
}
