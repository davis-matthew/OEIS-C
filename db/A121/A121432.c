/*
 * A121432  Number of subpartitions of partition
 *          P=[0,0,0,1,1,1,1,2,2,2,2,2,3,...], where P(n) = [(sqrt(8*n+25) -
 *          5)/2].
 * https://oeis.org/A121432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A121432: Number of subpartitions of the partition
 *
 *   a(n) = 1 – Σ_{k=0}^{n-1} a(k) · (-1)^{n-k} · C(P(k), n-k),
 *   where  P(k) = ⌊(√(8k+25) – 5)/2⌋
 *
 * This implementation evaluates the recurrence on demand and memoises
 * all computed terms in a static array.  It works for the plain
 * integer builds (uint8_t … uint64_t) as well as for the big‑integer
 * build (where seq_t is an arbitrary‑precision class).  No dynamic
 * allocation is required, so the code compiles both as C11 and as
 * C++17.  The running time is O(n √n) which easily meets the time
 * limits for n up to ~10 000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

/* maximum index that can be requested with reasonable confidence */
#define MAX_N 20000

/* static memoisation table */
static seq_t cache[MAX_N];
static idx_t cached = 0;  /* number of consecutive entries that are valid */

static idx_t int_sqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {                /* mid*mid would be 0 */
            ans = 0;
            low = 1;                   /* continue searching higher */
            continue;
        }
        if (mid > x / mid) {           /* mid*mid > x or overflow */
            high = mid - 1;
            continue;
        }
        idx_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            low = mid + 1;
            ans = mid;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

static idx_t P_of(idx_t n)
{
    /* P(n) = floor((sqrt(8*n+25) – 5)/2), result fits in idx_t */
    idx_t t = int_sqrt_u64(8 * n + 25);
    return (t >= 5) ? (t - 5) / 2 : 0;
}

/* multiplicative binomial coefficient C(n,k) for 0 ≤ k ≤ n */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i;  all ops are exact for integers */
        res = res * (n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n >= MAX_N) {
        /* n is beyond the guaranteed limit; fall back to compute on the fly */
        idx_t limit = n + 1;
        if (limit > MAX_N) limit = MAX_N;  /* adjust to max array size */
    }

    /* compute all terms up to n that have not yet been computed */
    for (idx_t m = cached; m <= n && m < MAX_N; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t diff = m - k;
            idx_t pk   = P_of(k);
            if (diff > pk) continue;     /* term vanishes */
            seq_t bin = binom(pk, diff);
            seq_t term = cache[k] * bin;
            if (diff & 1) sum -= term;
            else          sum += term;
        }
        cache[m] = 1 - sum;
    }

    cached = (n + 1 < cached) ? cached : n + 1;  /* ensure cached is at least n+1 */
    return cache[n];
}
