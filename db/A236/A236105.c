/*
 * A236105  Numbers whose proper divisors are partition numbers.
 * https://oeis.org/A236105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A236105: numbers whose proper divisors are partition numbers
   Computed by incremental search, with partition numbers generated
   on demand via the pentagonal‑number theorem. */
/* seq_t and idx_t are provided by the build */

/* Standard headers */
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Partition number table: p(0)=1, p(1)=1, p(2)=2, ...                */
/* ------------------------------------------------------------------ */
static seq_t *dp = NULL;          /* array storing partition numbers   */
static size_t dp_len = 0;         /* number of elements computed       */
static size_t dp_cap = 0;         /* allocated capacity                */

static void ensure_dp_cap(size_t needed) {
    if (dp_cap >= needed) return;
    size_t new_cap = dp_cap ? dp_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    dp = tmp;
    dp_cap = new_cap;
}

/* Extend dp so that p(k) > max_val is guaranteed for some k.
   That is, compute partition numbers until the last one exceeds max_val.
   Because partition numbers grow fast, this is very quick. */
static void extend_partition_values(seq_t max_val) {
    if (dp_len == 0) {                /* initialise with p(0)=1 */
        ensure_dp_cap(4);
        dp[0] = 1;
        dp_len = 1;
    }
    /* compute subsequent partition numbers */
    while (dp_len > 0 && dp[dp_len - 1] <= max_val) {
        idx_t n = dp_len;             /* index for new partition number */
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            /* generalized pentagonal numbers */
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > n) break;
            sum += (k & 1) ? dp[n - g1] : -dp[n - g1];
            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= n)
                sum += (k & 1) ? dp[n - g2] : -dp[n - g2];
        }
        ensure_dp_cap(dp_len + 1);
        dp[dp_len] = sum;
        dp_len++;
    }
}

/* Check whether x is a partition number (i.e. x = p(k) for some k). */
static bool is_partition(seq_t x) {
    size_t lo = 0, hi = dp_len;        /* hi is exclusive */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t v = dp[mid];
        if (v == x) return true;
        if (v < x) lo = mid + 1;
        else hi = mid;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Sequence cache: stores the sequence values found so far              */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;          /* array of results  */
static size_t cache_len = 0;         /* number of elements stored */
static size_t cache_cap = 0;         /* allocated capacity          */
static idx_t next_candidate = 2;     /* next integer to test          */

static void ensure_cache_cap(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Test whether n has all proper divisors as partition numbers. */
static bool good_candidate(idx_t n) {
    if (n < 2) return false;       /* sequence starts at 2 */
    seq_t max_div = (seq_t)(n - 1);
    extend_partition_values(max_div);
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;
        if (!is_partition((seq_t)d)) return false;
        idx_t e = n / d;
        if (e < n && e != d) {
            if (!is_partition((seq_t)e)) return false;
        }
    }
    return true;
}

/* Grow the cache so that it contains at least n terms. */
static void ensure_cache(idx_t n) {
    while (cache_len < (size_t)n) {
        while (true) {
            if (good_candidate(next_candidate)) {
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate;               /* move past the added value */
                break;
            }
            ++next_candidate;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset < 1 is undefined, return 0 safely */
    ensure_cache(n);
    return cache[n - 1];           /* cache is 0‑based, a(1) stored at index 0 */
}
