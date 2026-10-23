/*
 * A160837  G.f.: (1+38*x+262*x^2+475*x^3+254*x^4+37*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + n*(n+1)*(89*n^4+183*n^3+427*n^2+333*n+288)/60
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 37, u64 n <= 1521,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 0u)
        return (seq_t)1;

    /* Convert index to a larger integer type for intermediate
       computations.  All intermediate operations use idx_t (uint64_t)
       until the final division which requires a wider type. */
    idx_t nn   = n;
    idx_t n2   = nn * nn;          /* n^2  */
    idx_t n3   = n2 * nn;          /* n^3  */
    idx_t n4   = n2 * n2;          /* n^4  */

    /* Polynomial part: 89*n^4 + 183*n^3 + 427*n^2 + 333*n + 288   */
    idx_t poly = 89 * n4 + 183 * n3 + 427 * n2 + 333 * nn + 288;

    /* Use a 128‑bit integer for the product to avoid overflow.
       The product is exact because the division by 60 is guaranteed
       to yield an integer. */
    __int128 prod = (__int128)nn * (nn + 1);  /* n*(n+1)   */
    prod *= poly;                             /* *poly      */
    prod /= 60;                                /* /60        */

    /* Convert back to seq_t and add the constant term. */
    seq_t result = (seq_t)prod + (seq_t)1;
    return result;
}
