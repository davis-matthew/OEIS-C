/*
 * A053528  Number of bipartite graphs with 5 edges on nodes {1..n}.
 * https://oeis.org/A053528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-4)*(n-3)*(n-2)*(n-1)*n*(n^5 + 5*n^4 + 5*n^3 - 85*n^2 -
 *            374*n - 960)/3840
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 21, u64 n <= 193, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic. */
    seq_t N = (seq_t)n;

    /* For n < 5 the product contains a zero factor. */
    if (N < 5U) return 0;

    /* Factor A = (n-4)*(n-3)*(n-2)*(n-1)*n   */
    seq_t a_factor =
        (N - 4U) *
        (N - 3U) *
        (N - 2U) *
        (N - 1U) *
        N;

    /* Polynomial factor B = n^5 + 5*n^4 + 5*n^3 - 85*n^2 - 374*n - 960 */
    seq_t n2 = N * N;                     /* n^2   */
    seq_t n3 = n2 * N;                    /* n^3   */
    seq_t n4 = n3 * N;                    /* n^4   */
    seq_t n5 = n4 * N;                    /* n^5   */

    seq_t b_factor =
        n5 +       /* n^5          */
        5U * n4 +  /* 5*n^4        */
        5U * n3 -  /* 5*n^3        */
        85U * n2 - /* 85*n^2       */
        374U * N - /* 374*n        */
        960U;      /* 960           */

    /* Denominator 3840 = 2^8 * 3 * 5 */
    seq_t denom = 3840U;

    /* Reduce factor A by common powers of 2, 3, 5 */
    while ((denom % 2U) == 0 && (a_factor % 2U) == 0) {
        a_factor /= 2U;
        denom /= 2U;
    }
    while ((denom % 3U) == 0 && (a_factor % 3U) == 0) {
        a_factor /= 3U;
        denom /= 3U;
    }
    while ((denom % 5U) == 0 && (a_factor % 5U) == 0) {
        a_factor /= 5U;
        denom /= 5U;
    }

    /* Reduce factor B by remaining powers of 2, 3, 5 */
    while ((denom % 2U) == 0 && (b_factor % 2U) == 0) {
        b_factor /= 2U;
        denom /= 2U;
    }
    while ((denom % 3U) == 0 && (b_factor % 3U) == 0) {
        b_factor /= 3U;
        denom /= 3U;
    }
    while ((denom % 5U) == 0 && (b_factor % 5U) == 0) {
        b_factor /= 5U;
        denom /= 5U;
    }

    /* At this point denom should be 1. The result is the product. */
    seq_t result = a_factor * b_factor;
    return result;
}
