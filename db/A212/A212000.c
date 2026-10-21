/*
 * A212000  Triangle read by rows: T(n,k) is the total number of parts in the
 *          last n-k+1 section of the modular table of partitions of n.
 * https://oeis.org/A212000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) = A006128(r) - A006128(k-1) where (r,k) are the
 *            row and column (1‑based) of the n‑th term in the triangle read
 *            by rows
 * Range:     u8 n <= 55, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------------------------------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* -------------------------------------------------------------------- */
/* integer square root of an unsigned 64‑bit value, floor */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1 << 32; /* sqrt(2^64) <= 2^32 */
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > x / mid)      /* mid*mid > x, avoid overflow */
            high = mid - 1;
        else
            low = mid + 1;
    }
    return high;
}

/* -------------------------------------------------------------------- */
/* Max index sufficient for n ≤ 10000 : upper row around 142, safety margin */
#define MAX_N   2000   /* partitions needed up to this value */

/* partition numbers p[n] and A006128(n) (total parts in all partitions of n) */
static seq_t p[MAX_N + 1];
static seq_t part_sum[MAX_N + 1];
static bool init_done = false;

/* -------------------------------------------------------------------- */
/* one‑time precomputation of p and part_sum */
static void init_data(void)
{
    /* partition numbers: coin‑change DP, only addition */
    p[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAX_N; ++i) {
        for (idx_t j = i; j <= MAX_N; ++j) {
            p[j] += p[j - i];
        }
    }

    /* A006128(n) = sum_{j=1..n} sum_{k=1..⌊n/j⌋} p[n - k*j] */
    part_sum[0] = 0;
    for (idx_t n = 1; n <= MAX_N; ++n) {
        seq_t sum = 0;
        for (idx_t j = 1; j <= n; ++j) {
            idx_t kmax = n / j;
            for (idx_t k = 1; k <= kmax; ++k) {
                sum += p[n - k * j];
            }
        }
        part_sum[n] = sum;
    }
    init_done = true;
}

/* -------------------------------------------------------------------- */
/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* sequence is defined for n ≥ 1 */
        return 0;

    if (!init_done)
        init_data();

    /* find row r such that T(r-1) < n ≤ T(r), where T(r)=r*(r+1)/2 */
    idx_t sq = isqrt_u64(8 * n + 1);
    idx_t r  = (sq - 1) / 2;
    if (r * (r + 1) / 2 < n)
        ++r;

    /* column k (1‑based) */
    idx_t k = n - (r - 1) * r / 2;

    /* safety: r and k-1 should not exceed MAX_N for n ≤ 10000 */
    if (r > MAX_N || k == 0 || k - 1 > MAX_N)
        return 0;        /* out of precomputed range */

    return part_sum[r] - part_sum[k - 1];
}
