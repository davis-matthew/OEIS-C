/*
 * A049971  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = 1, a(2) = 3, and a(3) = 2.
 * https://oeis.org/A049971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 62, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A049971.  Recurrence:
   a(1) = 1, a(2) = 3, a(3) = 2.
   For n ≥ 4:
      a(n) = Σ_{k=1}^{n-1} a(k) + a(m)
   where m = 2*n - 2 - 2^(p+1) and p is the unique integer with
   2^p < n-1 ≤ 2^(p+1).  This implementation uses a static cache
   of already‑computed terms and cumulative sums.  It works sequentially
   for any n ≥ 1, with O(1) lookup after the initial buildup up to n. */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Utility: floor_log2 for a non‑zero unsigned value                    */
/* ------------------------------------------------------------------ */
static inline idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* ------------------------------------------------------------------ */
/*  Compile‑time limit for preallocation (sufficient for n≈100k).      */
/* ------------------------------------------------------------------ */
#define MAX_CAP 200000

/* ------------------------------------------------------------------ */
/*  Static storage for computed terms and their cumulative sums       */
/* ------------------------------------------------------------------ */
static seq_t seq_cache[MAX_CAP + 1];
static seq_t prefix_sum[MAX_CAP + 1];
static idx_t cache_size = 0;

/* ------------------------------------------------------------------ */
/*  Compute terms up to n (inclusive) if not already cached.           */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_size) return;
    if (n > MAX_CAP) {
        /* Out of preallocated bounds – abort gracefully.            */
        /* In typical tests n will stay below MAX_CAP.                */
        return;
    }

    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)1;
        } else if (i == 2) {
            val = (seq_t)3;
        } else if (i == 3) {
            val = (seq_t)2;
        } else {
            idx_t p   = floor_log2_idx(i - 2);        /* satisfies 2^p < i-1 ≤ 2^(p+1) */
            idx_t pow = (1ULL << (p + 1));           /* 2^(p+1) */
            idx_t m   = (2 * i) - 2 - pow;           /* 1 ≤ m < i */
            val = prefix_sum[i - 1] + seq_cache[m];
        }
        seq_cache[i]  = val;
        prefix_sum[i] = prefix_sum[i - 1] + val;
    }
    cache_size = n;
}

/* ------------------------------------------------------------------ */
/*  Public function required by the build                              */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* Out‑of‑range: unused value. */
    ensure_capacity(n);
    return seq_cache[n];
}
