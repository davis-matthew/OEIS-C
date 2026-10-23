/*
 * A189011  Zero-one sequence based on triangular numbers: a(A000217(k))=a(k);
 *          a(A014132(k))=1-a(k); a(1)=0.
 * https://oeis.org/A189011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(k) if 8*n+1 is a perfect square, otherwise a(n) =
 *            1-a(k), where k = (sqrt(8*n+1)-1)/2 (floor when not a triangle),
 *            and a(1) = 0. The function works for any n ≥ 1 and memoizes
 *            implicitly by recomputing from n down to 1. Only integer
 *            arithmetic is used
 * Range:     u8 n <= 135, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(x)) for uint64_t x.
   Uses binary search with a check to avoid overflow. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1ULL << 32;   /* high is ≥ sqrt(UINT64_MAX) */
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        /* mid*mid may overflow, so test mid <= x/mid */
        if (mid == 0 || mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* avoid infinite loop */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    seq_t flip = 0;                /* number of flips modulo 2 */

    while (n > 1) {
        idx_t t = 8 * n + 1;       /* 8*n + 1   (fits in idx_t for n ≤ ~10⁶⁴) */
        idx_t s = isqrt_u64(t);    /* floor(sqrt(8*n+1)) */
        idx_t tri = (s - 1) / 2;   /* floor((sqrt(8*n+1)-1)/2) */

        if (s * s == t) {         /* n is a triangular number */
            n = tri;              /* predecessor k = tri */
        } else {                  /* n is not triangular */
            n -= tri;             /* predecessor k = n - tri */
            flip ^= 1;            /* flip once */
        }
    }

    return (seq_t)flip;           /* a(1) = 0, so the accumulated flips give a(n) */
}
