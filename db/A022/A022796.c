/*
 * A022796  Place where n-th 1 occurs in A023134.
 * https://oeis.org/A022796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Σ_{k=0}^{n-1} ⌊ k / π ⌋ (equivalently, a(n) = n +
 *            Σ_{k=1}^{n} ⌊ (n-k) / π ⌋ )
 * Range:     u8 n <= 38, u16 n <= 640, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const unsigned long long SCALE = 1000000000000ULL;   /* 10^12 */
static const unsigned long long PINT  = 3141592653589ULL;   /* floor(π * SCALE) */

static idx_t maxComputedN = 0;                /* largest n for which cumulative[] is ready */
static seq_t *cumulative   = NULL;            /* cumulative[i] = Σ_{k=0}^{i} ⌊k/π⌋ */
static idx_t cumulativeCap = 0;               /* capacity of cumulative array */

static idx_t ceilSize   = 0;                  /* number of computed ceil(kπ) values */
static seq_t *ceilArr   = NULL;               /* ceilArr[k] = ⌈kπ⌉ */
static idx_t ceilCap    = 0;                  /* capacity of ceilArr */

/* Ensure cumulative array has capacity at least 'needed' */
static void ensure_cumulative_cap(idx_t needed)
{
    if (needed <= cumulativeCap)
        return;
    idx_t newCap = cumulativeCap ? cumulativeCap * 2 : 16;
    while (newCap < needed)
        newCap *= 2;
    cumulative = (seq_t *)realloc(cumulative, newCap * sizeof(seq_t));
    cumulativeCap = newCap;
}

/* Ensure ceilArr has capacity at least 'needed' indices (0‑based) */
static void ensure_ceil_cap(idx_t needed)
{
    if (needed <= ceilCap)
        return;
    idx_t newCap = ceilCap ? ceilCap * 2 : 16;
    while (newCap <= needed)
        newCap *= 2;
    ceilArr = (seq_t *)realloc(ceilArr, newCap * sizeof(seq_t));
    ceilCap = newCap;
}

/* Compute ceil(kπ) for all k in [ceilSize, needed-1] */
static void extend_ceil_arr(idx_t needed)
{
    if (needed <= ceilSize)
        return;
    for (idx_t k = ceilSize; k < needed; ++k) {
        /* product = k * PINT; use 128‑bit to avoid overflow */
        unsigned __int128 prod = (unsigned __int128)k * PINT;
        unsigned __int128 ceilk = (prod + SCALE - 1) / SCALE;
        if (ceilSize >= ceilCap) {
            /* enlarge the array (should not happen due to ensure_ceil_cap) */
            idx_t newCap = ceilCap ? ceilCap * 2 : 16;
            ceilArr = (seq_t *)realloc(ceilArr, newCap * sizeof(seq_t));
            ceilCap = newCap;
        }
        ceilArr[ceilSize++] = (seq_t)ceilk;
    }
}

/* Largest k such that ceilArr[k] ≤ j */
static idx_t floor_j(idx_t j)
{
    idx_t lo = 0;
    idx_t hi = ceilSize - 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (ceilArr[mid] <= j)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* not defined for n=0, return 0 gracefully */
        return 0;

    /* Determine how many ceil(kπ) values are needed for j up to n‑1 */
    unsigned __int128 tmp = (unsigned __int128)(n - 1) * SCALE;
    idx_t neededK = (idx_t)(tmp / PINT) + 2;      /* +2 for safety and k=0 */

    /* Ensure arrays are large enough */
    ensure_ceil_cap(neededK + 1);
    extend_ceil_arr(neededK + 1);

    /* Compute cumulative sums up to n‑1 if not already done */
    if (maxComputedN < n) {
        ensure_cumulative_cap(n);
        for (idx_t j = maxComputedN; j < n; ++j) {
            idx_t f = floor_j(j);          /* floor(j/π) */
            seq_t prev = (j == 0) ? 0 : cumulative[j - 1];
            cumulative[j] = prev + f;
        }
        maxComputedN = n;
    }

    seq_t sum = cumulative[n - 1];
    return n + sum;
}
