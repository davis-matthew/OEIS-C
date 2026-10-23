/*
 * A160835  G.f.: (1+44*x+339*x^2+630*x^3+323*x^4+42*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n*(n+1)*(23*n^4+48*n^3+98*n^2+73*n+58)/12
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 35, u64 n <= 1457,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to an unsigned 64‑bit value for safe arithmetic */
    unsigned long long N = (unsigned long long)n;

    /* use 128‑bit intermediate storage to avoid overflow for n up to 10 000 */
    __uint128_t n128 = N;
    __uint128_t n2   = n128 * n128;
    __uint128_t n3   = n2 * n128;
    __uint128_t n4   = n3 * n128;

    /* polynomial: 23*n^4 + 48*n^3 + 98*n^2 + 73*n + 58 */
    __uint128_t poly = (__uint128_t)23 * n4
                     + (__uint128_t)48 * n3
                     + (__uint128_t)98 * n2
                     + (__uint128_t)73 * n128
                     + (__uint128_t)58;

    /* compute n*(n+1)*poly / 12 + 1 */
    __uint128_t prod = n128 * (n128 + 1);
    prod = prod * poly;
    prod = prod / (__uint128_t)12;
    prod = prod + (__uint128_t)1;

    /* truncate to the build's seq_t type */
    return (seq_t)prod;
}
