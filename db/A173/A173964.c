/*
 * A173964  Sequence derived from a memorization technique.
 * https://oeis.org/A173964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173964:
   The sequence is obtained by writing the base strings 12,123,1234,…,
   and then appending for each base string the suffixes of length 2,
   3, …, up to (length – 1).  The resulting stream of digits
   (1 2|1 2 3 2 3|1 2 3 4 3 4 2 3 4|…) defines a(n). */

#include <stdint.h>

/* Forward declaration of helper that calculates the cumulative
   number of digits up to and including row M (row 2 has length 2). */
static inline idx_t cumulative(idx_t M)
{
    if (M < 2) return 0;
    /* S(M) = M*(M+1)*(M+2)/6 - M   (derived in analysis) */
    unsigned __int128 m = M;
    unsigned __int128 val = m * (m + 1) * (m + 2) / 6 - m;
    return (idx_t)val;
}

/* Compute a(n) for the OEIS sequence A173964.  Assumes n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1, return 0 */

    /* Find the row M such that S(M) ≥ n.  Use exponential increase
       to bound the search range, then binary search. */
    idx_t lo = 2, hi = 2;
    while (cumulative(hi) < n) hi <<= 1;   /* double until hi is large enough */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (cumulative(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t M = lo;                     /* the row containing position n */

    /* Position within row M (1‑based). */
    idx_t prev = (M == 2) ? 0 : cumulative(M - 1);
    idx_t offset = n - prev;          /* 1 ≤ offset ≤ L(M) */

    /* Row M contains first M digits 1…M, then suffixes of length 2…M‑1. */
    if (offset <= M) {
        return (seq_t)offset;         /* position in the initial part */
    }

    /* Position within the suffix portion. */
    idx_t r = offset - M;             /* 1‑based index among suffix digits */
    for (idx_t k = 2; k <= M - 1; ++k) {
        if (r <= k) {
            /* Digit at offset r in suffix of length k is
               M - k + r.  (indices 1‑based) */
            return (seq_t)(M - k + r);
        }
        r -= k;                       /* skip entire suffix of this length */
    }

    /* Should never reach here because offset <= L(M). */
    return 0;
}
