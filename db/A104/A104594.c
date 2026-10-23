/*
 * A104594  A129760/2.
 * https://oeis.org/A104594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(1) = 0.
 * For odd n ≥ 1, a(n) = (n – 1) / 2.
 * For even n, write n = 2^t · m with m odd and t ≥ 1.
 * Then a(n) = 2^(t–1) · (m – 1).  (If m = 1, a(n) = 0.)
 *
 * This implementation evaluates the formula directly in O(1) time.
 */

seq_t a(idx_t n)
{
    /* Handle odd n – trivial case. */
    if (n & 1ULL) {
        return (seq_t)((n - 1ULL) / 2ULL);
    }

    /* Even n: factor out the largest power of two. */
    idx_t t = 0;
    idx_t m = n;
    while ((m & 1ULL) == 0ULL) {
        m >>= 1ULL;
        ++t;
    }

    /* If m == 1, the value is zero (n is a power of 2). */
    if (m == 1ULL) {
        return (seq_t)0;
    }

    /* Compute 2^(t-1) * (m-1). */
    return (seq_t)((m - 1ULL) << (t - 1ULL));
}
