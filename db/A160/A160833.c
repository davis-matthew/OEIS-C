/*
 * A160833  G.f.: (1+62*x+569*x^2+1086*x^3+521*x^4+56*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + n*(n+1)*(287*n^4 + 619*n^3 + 1021*n^2 + 689*n +
 *            444)/90
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 32, u64 n <= 1339,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the polynomial in a type large enough to hold
       intermediate values.  If the compiler provides unsigned 128‑bit
       integers, use them; otherwise operate directly on seq_t. */
#ifdef __SIZEOF_INT128__
    __uint128_t nn = n;
    __uint128_t P  = 287;
    P = P * nn + 619;
    P = P * nn + 1021;
    P = P * nn + 689;
    P = P * nn + 444;
    __uint128_t prod = nn * (nn + 1);
    prod *= P;
    prod /= 90;
    prod += 1;
    return (seq_t)prod;
#else
    seq_t sn  = (seq_t)n;
    seq_t P   = 287;
    P = P * sn + 619;
    P = P * sn + 1021;
    P = P * sn + 689;
    P = P * sn + 444;

    seq_t prod = sn * (sn + 1);
    prod = prod * P;
    prod = prod / 90;
    prod = prod + 1;
    return prod;
#endif
}
