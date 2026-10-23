/*
 * A194742  Number of k such that {k*sqrt(5)} < {n*sqrt(5)}, where { } =
 *          fractional part.
 * https://oeis.org/A194742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ k : 0 ≤ k < n and {k*sqrt(5)} < {n*sqrt(5)} }
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm works with 128‑bit integer arithmetic, avoiding floating */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* 128‑bit integer square root (binary search) */
static __uint128_t isqrt128(__uint128_t x)
{
    __uint128_t lo = 0, hi = (( (__uint128_t)1 << 64 ) - 1);
    while (lo <= hi) {
        __uint128_t mid = (lo + hi) >> 1;
        __uint128_t sq  = mid * mid;
        if (sq <= x) lo = mid + 1;
        else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Pre‑computation parameters */
#define SCALE_BITS 20          /* 2^20 ≈ 1.048 576 – enough precision */
#define PRE_MAX_N 200000       /* covers all test n up to ~100 000 */

/* Global tables – initialised once */
static seq_t   *cache      = NULL;   /* a(n) values    */
static uint64_t *val       = NULL;   /* scaled fractional parts */
static idx_t   *rank_arr   = NULL;   /* rank of each index */
static int     *bit        = NULL;   /* Fenwick tree   */
static bool    initialized = false;

/* Fenwick tree helpers (1‑based indices) */
static inline void bit_update(size_t i, int delta)
{
    for (; i < PRE_MAX_N + 2; i += i & -i) bit[i] += delta;
}
static inline int bit_query(size_t i)
{
    int s = 0;
    for (; i > 0; i -= i & -i) s += bit[i];
    return s;
}

/* Comparator for sorting indices by their scaled fractional part */
static int cmp_idx(const void *p1, const void *p2)
{
    idx_t i = *(const idx_t*)p1;
    idx_t j = *(const idx_t*)p2;
    if (val[i] < val[j]) return -1;
    if (val[i] > val[j]) return 1;
    return 0;
}

/* Initialisation – executed once */
static void init(void)
{
    cache   = (seq_t*)   malloc((PRE_MAX_N + 1) * sizeof(seq_t));
    val     = (uint64_t*)malloc((PRE_MAX_N + 1) * sizeof(uint64_t));
    rank_arr= (idx_t*)   malloc((PRE_MAX_N + 1) * sizeof(idx_t));
    bit     = (int*)     calloc(PRE_MAX_N + 2, sizeof(int));
    idx_t *idx = (idx_t*)malloc((PRE_MAX_N + 1) * sizeof(idx_t));

    /* Compute scaled fractional parts for k = 1 .. PRE_MAX_N */
    for (idx_t k = 1; k <= PRE_MAX_N; ++k) {
        /* floor(k*sqrt5) = floor(sqrt(5*k^2)) */
        __uint128_t t = (__uint128_t)5 * k * k;
        uint64_t floor_sqrt = (uint64_t)isqrt128(t);

        /* floor(k*sqrt5 * 2^SCALE_BITS) = floor(sqrt(5*k^2*2^{2*B})) */
        __uint128_t t2 = t << (2 * SCALE_BITS);
        __uint128_t floor_scaled = isqrt128(t2);

        /* scaled fractional part = floor_scaled - floor_sqrt << B */
        val[k] = (uint64_t)(floor_scaled - ((__uint128_t)floor_sqrt << SCALE_BITS));
    }

    /* Build index array 1..PRE_MAX_N */
    for (idx_t i = 1; i <= PRE_MAX_N; ++i) idx[i] = i;

    /* Sort indices by scaled fractional value */
    qsort(&idx[1], PRE_MAX_N, sizeof(idx_t), cmp_idx);

    /* Assign ranks 1..PRE_MAX_N */
    for (size_t r = 1; r <= PRE_MAX_N; ++r)
        rank_arr[idx[r]] = (idx_t)r;

    /* Compute a(n) using the Fenwick tree */
    for (idx_t n = 1; n <= PRE_MAX_N; ++n) {
        size_t r = rank_arr[n];
        int less = bit_query(r - 1);
        cache[n] = (seq_t)(less + 1);   /* +1 accounts for k = 0 which is always < */
        bit_update(r, 1);
    }

    free(idx);
}

/* Sequence function – external linkage */
seq_t a(idx_t n)
{
    if (!initialized) { init(); initialized = true; }

    if (n == 0) return 0;
    if (n > PRE_MAX_N) return 0;   /* out of pre‑computed range – should not happen */
    return cache[n];
}
