/*
 * A051744  a(n) = n*(n+1)*(n^2+5*n+18)/24.
 * https://oeis.org/A051744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n^2+5*n+18)/24
 * Range:     u8 n <= 7, u16 n <= 33, u32 n <= 565, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence grows rapidly; for the required n up to ~10⁴
       a 64‑bit unsigned integer is sufficient for the intermediate
       computations.  For builds where seq_t is larger (128‑bit or
       arbitrary precision) casting back to seq_t keeps the result
       exact for all values that fit in seq_t. */
    unsigned long long nn = (unsigned long long)n;          /* avoid idx_t size issues */
    unsigned long long sq = nn * nn;                       /* n² */
    unsigned long long prod = nn * (nn + 1);                /* n·(n+1) */
    prod *= (sq + 5 * nn + 18);                            /* times (n²+5n+18) */
    prod /= 24;                                            /* divide by 24 */
    return (seq_t)prod;                                    /* cast to sequence type */
}
