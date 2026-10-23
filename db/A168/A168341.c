/*
 * A168341  The lexicographically earliest injective sequence of nonnegative
 *          integers such that a(a(n)) is a square for all n>=0.
 * https://oeis.org/A168341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+1 // if (n - k^2) is odd a(n) = ((n + k)/2)^2 // if (n
 *            - k^2) is even where k = floor(sqrt(n)), i.e. the largest
 *            integer with k^2 <= n
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t n)
{
    /* Compute floor(sqrt(n)) for 0 <= n <= 2^64-1.
       Uses a binary search that avoids overflow. */
    idx_t lo = 0;
    idx_t hi = n;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);      /* upper mid */
        if (mid <= n / mid) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t k    = isqrt_u64(n);          /* largest square root <= n */
    idx_t diff = n - k * k;             /* n - k^2, 0 <= diff < 2k+1 */

    if (diff & 1ULL) {                  /* odd difference: return n+1 */
        return (seq_t)(n + 1);
    } else {                            /* even difference: produce a square */
        seq_t t = ((seq_t)n + (seq_t)k) / 2;  /* (n + k) / 2 */
        return t * t;                     /* square of the half */
    }
}
