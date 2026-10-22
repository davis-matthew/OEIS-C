/*
 * A005996  G.f.: 2*(1-x^3)/((1-x)^5*(1+x)^2).
 * https://oeis.org/A005996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)/4 if n even; a(n) = (n+1)^3/4 if n odd
 * Range:     u8 n <= 9, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    
    if (n & 1) {  // n is odd
        seq_t m = ns + 1;
        return m * m * m / 4;
    } else {  // n is even
        return ns * (ns + 1) * (ns + 2) / 4;
    }
}
