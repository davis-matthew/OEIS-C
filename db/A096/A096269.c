/*
 * A096269  a(n) = number of distinct palindromes of length n that occur in
 *          A096268.
 * https://oeis.org/A096269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 2 a(2) = 1 a(3) = 3 for n >= 4: if n even → a(n) = 0 if n
 *            odd (>=5) → let x be the power of 2 closest to n; if n > x →
 *            a(n) = 4 else → a(n) = 3 (This matches the recurrence
 *            a(n)=a(2n-1)=a(2n+1) for odd n>=5.)
 * Range:     u8 n <= 8703, u16 n <= 8703, u32 n <= 8703, u64 n <= 8703,
 *            u128 n <= 8703, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1U) return (seq_t)2;
    if (n == 2U) return (seq_t)1;
    if (n == 3U) return (seq_t)3;

    /* for n >= 4 */
    if ((n & 1ULL) == 0ULL)            /* even n */
        return (seq_t)0U;

    /* odd n >= 5 ----------------------------------------------------- */
    /* find largest power of two <= n */
    idx_t lo = 1ULL;
    while ((lo <= (UINT64_MAX >> 1U)) && ((lo << 1U) <= n))
        lo <<= 1U;

    /* compute distance to nearest lower and upper power of two */
    idx_t diff_lo = n - lo;
    idx_t diff_hi;
    if (lo <= (UINT64_MAX >> 1U)) {
        diff_hi = (lo << 1U) - n;   /* upper power exists */
    } else {
        diff_hi = UINT64_MAX;      /* no upper power – treat as far away */
    }

    /* nearest power of two */
    idx_t nearest = lo;
    if (diff_hi < diff_lo)
        nearest = lo << 1U;        /* use higher power */

    return (n > nearest) ? (seq_t)4U : (seq_t)3U;
}
