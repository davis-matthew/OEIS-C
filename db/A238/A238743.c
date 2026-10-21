/*
 * A238743  Triangle read by rows: T(n,k) =
 *          A059382(n)/(A059382(k)*A059382(n-k)).
 * https://oeis.org/A238743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of the triangle T(n,k) read by rows,
 *            where T(n,k) = A059382(n) / (A059382(k) * A059382(n-k));
 *            A059382(m) is the product of A059376(i) = J₃(i) for i = 1..m
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 28, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes J₃(n) using the smallest prime factor
   sieve, stores the prefix products A059382(n), and computes
   a(n) = T(row,col) from the flattened index. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* global small prime factor cache */
static uint64_t *spf_cache = NULL;
static idx_t  spf_limit = 0;

static void ensure_spf(idx_t lim) {
    if (lim <= spf_limit) return;
    free(spf_cache);
    spf_cache = (uint64_t*)malloc((size_t)(lim + 1) * sizeof(uint64_t));
    if (!spf_cache) exit(1);
    for (idx_t i = 0; i <= lim; ++i) spf_cache[i] = 0;
    spf_cache[1] = 1;
    for (idx_t i = 2; i <= lim; ++i) {
        if (spf_cache[i] == 0) {              /* i is prime */
            spf_cache[i] = i;
            if (i * i <= lim) {
                for (idx_t j = i * i; j <= lim; j += i)
                    if (spf_cache[j] == 0) spf_cache[j] = i;
            }
        }
    }
    spf_limit = lim;
}

/* compute A059376(n) = J₃(n) */
static seq_t compute_J3(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t result = (seq_t)n * (seq_t)n * (seq_t)n;   /* n³ */
    idx_t x = n;
    while (x > 1) {
        uint64_t p = spf_cache[x];
        while (x % p == 0) x /= p;                  /* distinct prime only */
        seq_t p3 = (seq_t)p * p * p;
        result = (result / p3) * (p3 - (seq_t)1);
    }
    return result;
}

/* caches for A059376 and A059382 prefixes */
static seq_t *J_cache   = NULL;     /* J_cache[i] = A059376(i)   */
static seq_t *prefix    = NULL;     /* prefix[i]  = A059382(i)   */
static size_t J_size = 0;

/* ensure values up to n are computed */
static void ensure_J_and_prefix(idx_t n) {
    if (n + 1 <= J_size) return;
    size_t newsize = n + 1;
    J_cache = (seq_t*)realloc(J_cache, newsize * sizeof(seq_t));
    prefix  = (seq_t*)realloc(prefix,  newsize * sizeof(seq_t));
    if (!J_cache || !prefix) exit(1);
    ensure_spf(n);
    if (J_size == 0) {
        J_cache[0] = (seq_t)1;
        prefix[0]  = (seq_t)1;
        J_size = 1;
    }
    for (idx_t i = J_size; i <= n; ++i) {
        seq_t jc = compute_J3(i);
        J_cache[i] = jc;
        if (i == 0) prefix[i] = (seq_t)1;
        else prefix[i] = prefix[i - 1] * jc;
    }
    J_size = newsize;
}

/* convert flattened index to row and column */
static void row_col_from_index(idx_t flat, idx_t *row, idx_t *col) {
    idx_t low = 0;
    idx_t high = (idx_t)(sqrtl((long double)2.0 * flat) + 2);
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid * (mid + 1) / 2 <= flat) low = mid;
        else high = mid - 1;
    }
    *row = low;
    *col = flat - low * (low + 1) / 2;
}

/* public entry point */
seq_t a(idx_t n) {
    idx_t r, k;
    row_col_from_index(n, &r, &k);
    ensure_J_and_prefix(r);
    seq_t denom = prefix[k] * prefix[r - k];
    return prefix[r] / denom;
}
