/*
 * A110691  Kekulé numbers for certain benzenoids.
 * https://oeis.org/A110691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^3*(n+3)*(13*n^4 + 104*n^3 + 311*n^2 + 412*n +
 *            210)/5040
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 20, u64 n <= 266, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use 128‑bit unsigned integer for safe intermediate arithmetic.
       The product (n+1)*(n+2)^3*(n+3)*(13*n^4+...+210) fits in 128 bits
       for all n up to at least 10000 (which is well beyond the tested
       range).  No overflow occurs before the final division by 5040. */
    __uint128_t n128 = ( __uint128_t ) n;

    __uint128_t a = n128 + 1;           /* (n+1)          */
    __uint128_t b = n128 + 2;           /* (n+2)          */
    __uint128_t c = n128 + 3;           /* (n+3)          */

    /* d = 13*n^4 + 104*n^3 + 311*n^2 + 412*n + 210 */
    __uint128_t n2   = n128 * n128;         /* n^2   */
    __uint128_t n3   = n2   * n128;         /* n^3   */
    __uint128_t n4   = n3   * n128;         /* n^4   */
    __uint128_t d    = 13 * n4 + 104 * n3 + 311 * n2 + 412 * n128 + 210;

    /* Compute numerator: (n+1)*(n+2)^3*(n+3)*d */
    __uint128_t numerator = a;
    numerator *= b;       /* a*(n+2)          */
    numerator *= b;       /* a*(n+2)^2        */
    numerator *= b;       /* a*(n+2)^3        */
    numerator *= c;       /* * (n+3)          */
    numerator *= d;       /* * d              */

    /* Final division by 5040 (7!) */
    numerator /= 5040;

    return (seq_t)numerator;
}
