/*
 * A122799  A P_7-stuttered arithmetic progression with a(n+1)=a(n) if n is
 *          not a heptagonal number, a(n+1)=a(n)+2 otherwise.
 * https://oeis.org/A122799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 - 2*floor((3 + floor(sqrt(9+40*(n-1))))/10)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: integer square root of a 64‑bit unsigned integer. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = x;
    idx_t ans  = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid == 0) { /* avoid division by zero in x/mid */
            low = 1;
            continue;
        }
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n >= 1 by the sequence definition */
    idx_t m   = n - 1;                         /* use n-1 for counting before n */
    idx_t D   = (idx_t)9 + (idx_t)40 * m;     /* 9 + 40*(n-1) */
    idx_t sqrt_D = isqrt_u64(D);              /* floor(sqrt(D)) */
    idx_t k   = (sqrt_D + (idx_t)3) / (idx_t)10; /* count of heptagonal numbers <= n-1 */

    /* a(n) = 2*n - 1 - 2*k */
    idx_t result_idx = (idx_t)2 * n - (idx_t)1 - (idx_t)2 * k;
    return (seq_t)result_idx;
}
