/*
 * A213479  Number of (w,x,y) with all terms in {0,...,n} and |w-x|+|x-y| =
 *          w+x+y.
 * https://oeis.org/A213479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^2+22*n+8)/8 for n even, (9*n^2+20*n+3)/8 for n odd
 * Range:     u8 n <= 13, u16 n <= 240, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a wider unsigned integer for intermediate arithmetic */
    unsigned long long N = (unsigned long long)n;
    unsigned long long r;

    if ((n & 1ULL) == 0)    /* even n */
        r = (9ULL * N * N + 22ULL * N + 8ULL) / 8ULL;
    else                    /* odd n */
        r = (9ULL * N * N + 20ULL * N + 3ULL) / 8ULL;

    return (seq_t)r;
}
