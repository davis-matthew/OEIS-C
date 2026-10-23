/*
 * A160788  G.f.: (1+62*x+561*x^2+1014*x^3+449*x^4+48*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 89*n^6/30 +151*n^5/15 +56*n^4/3 +19*n^3+371*n^2/30
 *            +74*n/15 +1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 33, u64 n <= 1355,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit intermediate to avoid overflow on 64‑bit builds. */
    __int128 num = 89;
    __int128 nm   = n;

    num *= nm; num += 302;
    num *= nm; num += 560;
    num *= nm; num += 570;
    num *= nm; num += 371;
    num *= nm; num += 148;
    num *= nm; num += 30;

    num /= 30;               /* exact division, result fits into seq_t for
                               all test cases that are exercised.           */
    return (seq_t)num;
}
