/*
 * A115994  Triangle read by rows: T(n,k) is number of partitions of n with
 *          Durfee square of size k (n>=1; 1<=k<=floor(sqrt(n))).
 * https://oeis.org/A115994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) is the flattened triangle of T(n,k) where T(n,k) =
 *            sum_{i=0}^{n-k^2} P_exact(i+k,k) * P_exact(n-k^2-i+k,k), and
 *            P_exact(m,k) = number of partitions of m into exactly k parts
 * Range:     u8 n <= 51, u16 n <= 200, u32 n <= 1017, u64 n <= 5900,
 *            u128 n <= 9463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --- integer square root ------------------------------------------------- */
static idx_t _floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = 1ULL << 32;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t sq = (__uint128_t)mid * ( __uint128_t)mid;
        if (sq <= n) low = mid; else high = mid - 1;
    }
    return low;
}

/* ------------------------------------------------------------------------- */
static idx_t *cumcnt = NULL;
static size_t cumsize = 0, cumcap = 0;

static void _ensure_cum(void)
{
    if (!cumcnt) {
        cumcap = 64;                     /* small initial capacity            */
        cumcnt = (idx_t *)malloc(cumcap * sizeof(idx_t));
        if (!cumcnt) exit(EXIT_FAILURE);
        cumcnt[0] = 0;
        cumsize = 1;
    } else if (cumsize == cumcap) {
        cumcap <<= 1;
        cumcnt = (idx_t *)realloc(cumcnt, cumcap * sizeof(idx_t));
        if (!cumcnt) exit(EXIT_FAILURE);
    }
}

/* grow cumcnt until it covers at least term index n */
static void _ensure_cumcnt(idx_t n)
{
    _ensure_cum();
    while (cumcnt[cumsize - 1] < n) {
        idx_t row  = cumsize;                     /* next row number (starting at 1) */
        idx_t kmax = _floor_isqrt_u64(row);
        cumcnt[row] = cumcnt[row - 1] + kmax;
        ++cumsize;
        if (cumsize > cumcap) _ensure_cum();
    }
}

/* ----------------------------------------------------------------------- */
/*  DP table for partitions into at most k parts:  P_atmost(n,k)            */
static seq_t *dp = NULL;
static idx_t   dp_kmax = 0;   /* maximum k stored (floor(sqrt(Nmax)))   */
static idx_t   dp_nmax = 0;   /* maximum n stored (max row)              */
static idx_t   dp_stride = 0; /* stride = dp_nmax + 1                    */

/* rebuild DP table up to max row = maxN */
static void _rebuild_dp(idx_t maxN)
{
    idx_t K      = _floor_isqrt_u64(maxN);      /* max k needed          */
    idx_t Nmax   = maxN;                        /* max n needed          */
    idx_t stride = Nmax + 1;                    /* columns for n 0..Nmax */
    size_t sz = (size_t)(K + 1) * (size_t)stride;
    seq_t *newdp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!newdp) exit(EXIT_FAILURE);

    /* base cases: k = 0 */
    for (idx_t n = 0; n <= Nmax; ++n)
        newdp[0 * stride + n] = (n == 0) ? 1 : 0;

    /* base cases: n = 0, k > 0 */
    for (idx_t k = 1; k <= K; ++k)
        newdp[k * stride + 0] = 1;

    /* recurrence: P_atmost(n,k) = P_atmost(n,k-1) + P_atmost(n-k,k) */
    for (idx_t k = 1; k <= K; ++k) {
        for (idx_t n = 1; n <= Nmax; ++n) {
            seq_t a = newdp[(k - 1) * stride + n];
            seq_t b = (n >= k) ? newdp[k * stride + (n - k)] : 0;
            newdp[k * stride + n] = a + b;
        }
    }

    /* replace old table */
    if (dp) free(dp);
    dp = newdp;
    dp_kmax = K;
    dp_nmax = Nmax;
    dp_stride = stride;
}

/* ensure DP covers at least rows up to N */
static void _ensure_dp(idx_t N)
{
    if (N <= dp_kmax) return;   /* current DP has sufficient k            */
    _rebuild_dp(N);
}

/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, guard                */

    _ensure_cumcnt(n);             /* make sure cumcnt up to n            */

    /* binary search for row R with cumcnt[R-1] < n <= cumcnt[R] */
    idx_t low = 1, high = (idx_t)(cumsize - 1);
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if (cumcnt[mid] >= n) high = mid;
        else low = mid + 1;
    }
    idx_t R = low;                 /* row number                        */
    idx_t prev = cumcnt[R - 1];
    idx_t k_rel = n - prev;         /* 1‑based column in row R           */

    idx_t row = R;
    idx_t k   = k_rel;

    _ensure_dp(row);

    idx_t m = row - k * k;          /* n - k^2                               */
    __uint128_t sum128 = 0;

    for (idx_t i = 0; i <= m; ++i) {
        /* P_exact(i+k,k) = P_atmost(i+k,k) - P_atmost(i+k,k-1) */
        seq_t p1 = dp[k * dp_stride + i + k] -
                   (k == 0 ? 0 : dp[(k - 1) * dp_stride + i + k]);

        seq_t p2 = dp[k * dp_stride + (m - i) + k] -
                   (k == 0 ? 0 : dp[(k - 1) * dp_stride + (m - i) + k]);

        __uint128_t prod = (__uint128_t)p1 * (__uint128_t)p2;
        sum128 += prod;
    }
    return (seq_t)sum128;
}
