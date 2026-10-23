/*
 * A160853  Expansion of
 *          (1+147*x+1230*x^2+1925*x^3+754*x^4+67*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n+1)*(1375*n^4+4022*n^3+6573*n^2+4582*n+1808)/240
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 29, u64 n <= 1214,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Promote the index to seq_t.
       Use a wider integer type for intermediates to avoid overflow. */
    __uint128_t N = ( __uint128_t ) n;

    /* polynomial terms */
    __uint128_t n2 = N * N;          /* n^2   */
    __uint128_t n3 = n2 * N;         /* n^3   */
    __uint128_t n4 = n3 * N;         /* n^4   */
    __uint128_t poly = ( __uint128_t )1375 * n4
                     + ( __uint128_t )4022 * n3
                     + ( __uint128_t )6573 * n2
                     + ( __uint128_t )4582 * N
                     + ( __uint128_t )1808;

    /* n*(n+1) */
    __uint128_t t = N * (N + 1);

    /* perform the division before converting back to seq_t */
    __uint128_t part = t * poly / 240;

    /* Return the final result, cast back to seq_t. */
    return (seq_t) (1 + part);
}
