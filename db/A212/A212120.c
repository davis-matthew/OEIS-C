/*
 * A212120  Triangle read by rows T(n,k), n>=1, k>=1, where T(n,k) is the sum
 *          of the divisors d of n with min(d, n/d) = k.
 * https://oeis.org/A212120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth entry of the triangle read by rows where *
 *            T(n,k) = sum of divisors d of n with min(d, n/d) = k. * The
 *            triangle can be described as: * For each row r (r ≥ 1) and
 *            column k (1 ≤ k ≤ ⌊√r⌋), * the entry in that cell equals * 2 *
 *            ⌊(r – k²) / k⌋ + 1 (an odd number). * The sequence a(n)...
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: largest x with x*x ≤ n */
static inline idx_t isqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = mid + 1;
            continue;
        }
        /* use division to avoid overflow */
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;   /* high is the floor of sqrt(n) */
}

/* F(r) = Σ_{i=1..r} floor(sqrt(i))  (capped at max value of idx_t) */
static inline idx_t F_of_r(idx_t r)
{
    if (r == 0) return 0;
    idx_t t = isqrt(r);                /* floor(sqrt(r)) */
    idx_t m = (t == 0) ? 0 : t - 1;    /* t-1 for the full blocks */

    /* compute sum_{k=1}^{m} k*(2k+1) using formulas:
     *   sum(k^2) = m(m+1)(2m+1)/6
     *   sum(k)   = m(m+1)/2
     *   sum k*(2k+1) = 2*sum(k^2) + sum(k)  */
    __int128 m128 = m;
    __int128 part1 = 2 * m128 * (m128 + 1) * (2 * m128 + 1) / 6;   /* 2*sum(k^2) */
    __int128 part2 = m128 * (m128 + 1) / 2;                         /* sum(k)   */
    __int128 sum   = part1 + part2;

    /* add the incomplete last block with value t */
    idx_t last_count = r - t * t + 1;   /* count of i with floor(sqrt(i)) = t */
    sum += (__int128)t * last_count;

    /* prevent overflow: cap at maximum idx_t value */
    const idx_t MAX_IDX = (idx_t)-1;
    if (sum > (__int128)MAX_IDX) return MAX_IDX;
    return (idx_t)sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, guard against n=0 */

    /* find smallest r with F(r) >= n using exponential search + binary search */
    idx_t lo = 1, hi = 1;
    while (F_of_r(hi) < n) {
        /* double hi, but stop at maximum idx_t to avoid overflow */
        if (hi > ((idx_t)-1) / 2) { hi = (idx_t)-1; break; }
        hi <<= 1;
    }
    /* binary search in [lo, hi] */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (F_of_r(mid) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t r = lo;                    /* row containing the target */

    /* number of earlier elements */
    idx_t prev = F_of_r(r - 1);
    idx_t k = n - prev;              /* column index (1‑based) in this row */

    /* compute the entry value */
    idx_t diff = r - k * k;
    idx_t t    = diff / k;
    idx_t odd  = 2 * t + 1;          /* odd number formula */

    return (seq_t)odd;
}
