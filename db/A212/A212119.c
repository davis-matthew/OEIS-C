/*
 * A212119  Triangle read by rows T(n,k), n>=1, k>=1, where T(n,k) is the
 *          number of divisors d of n with min(d, n/d) = k.
 * https://oeis.org/A212119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where r is the unique row with Σ_{i=1}^{r-1} ⌊√i⌋
 *            < n ≤ Σ_{i=1}^{r} ⌊√i⌋, k = n − Σ_{i=1}^{r-1} ⌊√i⌋, and T(r,k)
 *            is 0 if r mod k ≠ 0, 1 if r/k = k, 2 otherwise
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Integer floor square root, using 128‑bit arithmetic to avoid overflow. */
static inline idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* Cached cumulative sums of ⌊√i⌋ for rows 1..r.  cum[0] == 0. */
static idx_t *cum = NULL;         /* array of cumulative sums */
static size_t cum_cap = 0;        /* allocated capacity */
static idx_t cum_len = 0;         /* number of rows currently stored (>=0) */

/* Ensure the cumulative array contains sums up to row r needed for n. */
static void ensure_cum(idx_t target_sum)
{
    if (cum_cap == 0) {
        cum_cap = 16;
        cum = (idx_t *)malloc(cum_cap * sizeof(idx_t));
        if (!cum) return;      /* allocation failure: caller will return 0 */
        cum[0] = 0;
        cum_len = 0;
    }
    while (cum_len == 0 || cum[cum_len] < target_sum) {
        idx_t next_row = cum_len + 1;
        idx_t sqrt_r = isqrt(next_row);
        idx_t new_sum = cum[cum_len] + sqrt_r;

        if (cum_len + 1 >= cum_cap) {
            size_t new_cap = cum_cap << 1;
            idx_t *tmp = (idx_t *)realloc(cum, new_cap * sizeof(idx_t));
            if (!tmp) return;    /* allocation failure */
            cum = tmp;
            cum_cap = new_cap;
        }
        cum_len++;
        cum[cum_len] = new_sum;
    }
}

/* Main function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0, return 0 */

    /* Ensure the cumulative array reaches at least sum ≥ n. */
    ensure_cum(n);
    if (!cum) return 0; /* allocation failure */

    /* Binary search for the smallest r with cum[r] >= n. */
    idx_t lo = 1, hi = cum_len, r = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (cum[mid] >= n) {
            r = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    /* Position within row r */
    idx_t k = n - cum[r - 1];

    /* Compute T(r,k) according to the definition. */
    if (r % k != 0) return (seq_t)0;
    idx_t d = r / k;
    if (d == k) return (seq_t)1;
    return (seq_t)2;
}
