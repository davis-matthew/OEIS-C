/*
 * A122800  A P_4-stuttered arithmetic progression with a(n+1)=a(n) if n is
 *          square, a(n+1)=a(n)+2 otherwise.
 * https://oeis.org/A122800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 - 2*⌊√(n-1)⌋
 * Range:     u8 n <= 139, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence a(n+1)=a(n)+2 except when n is a perfect square,
   in which case a(n+1)=a(n).  The value is calculated via the closed‑form above. */

#include <stdint.h>   /* for idx_t and seq_t */

/* Integer square root: floor(sqrt(x)) for non‑negative x (idx_t). */
static idx_t isqrt_idx_t(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid != 0 && mid > x / mid) {
            hi = mid - 1;
        } else {
            ans = mid;
            lo = mid + 1;
        }
    }
    return ans;
}

/* Computes the A122800 sequence term at index n (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n<1, return 0 */

    idx_t t = n - 1;                        /* indices before n */
    idx_t s = isqrt_idx_t(t);              /* count of squares ≤ t */
    return (seq_t)( (seq_t)2*(seq_t)n - (seq_t)1 - (seq_t)2*s );
}
