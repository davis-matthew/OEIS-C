/*
 * A211538  Number of ordered triples (w,x,y) with all terms in {1,...,n} and
 *          2w=2n-2x-y.
 * https://oeis.org/A211538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n*n-26*n+25-(2*n-7)*(-1)^n)/16
 * Range:     u8 n <= 28, u16 n <= 420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider integer type to avoid intermediate overflow.
       The result fits in seq_t for all n that fit in seq_t.   */
    unsigned long long N = (unsigned long long)n;
    unsigned long long num;

    if (n & 1ULL) {                /* n is odd */
        /* For odd n: (6n^2 - 24n + 18) / 16   */
        num = 6ULL * N * N - 24ULL * N + 18ULL;
    } else {                        /* n is even */
        /* For even n: (6n^2 - 28n + 32) / 16  */
        num = 6ULL * N * N - 28ULL * N + 32ULL;
    }

    return (seq_t)(num / 16ULL);
}
