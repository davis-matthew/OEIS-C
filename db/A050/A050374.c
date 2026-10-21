/*
 * A050374  Number of ordered factorizations of n into composite factors.
 * https://oeis.org/A050374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of ordered factorizations of n into composite
 *            factors. Recurrence: a(1)=1; for n>1, a(n)=∑_{c composite, c|n}
 *            a(n/c)
 * Range:     u8 n <= 2879, u16 n <= 8865, u32 n <= 8865, u64 n <= 8865,
 *            u128 n <= 8865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static uint64_t *spf = NULL;          /* smallest prime factors */
static size_t spf_size = 0;

static inline bool is_composite(idx_t x)
{
    return x > 1 && spf[x] != x;
}

static void ensure_spf(idx_t n)
{
    if (n < spf_size) return;
    size_t new_sz = n + 1;
    uint64_t *new_spf = (uint64_t *)realloc(spf, new_sz * sizeof(uint64_t));
    if (!new_spf) abort();
    spf = new_spf;
    for (idx_t i = spf_size; i <= n; ++i) spf[i] = i;
    for (idx_t i = 2; i * i <= n; ++i)
        if (spf[i] == i)
            for (idx_t j = i * i; j <= n; j += i)
                if (spf[j] == j) spf[j] = i;
    spf_size = new_sz;
}

static seq_t *dp = NULL;          /* memoised values */
static unsigned char *seen = NULL;/* 0 = not computed, 1 = computed */
static size_t dp_size = 0;

static void ensure_dp(idx_t n)
{
    if (dp_size > n) return;
    size_t new_sz = n + 1;
    seq_t *new_dp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
    if (!new_dp) abort();
    unsigned char *new_seen = (unsigned char *)realloc(seen, new_sz * sizeof(unsigned char));
    if (!new_seen) abort();
    for (idx_t i = dp_size; i <= n; ++i) {
        new_dp[i] = (seq_t)0;
        new_seen[i] = 0;
    }
    dp = new_dp;
    seen = new_seen;
    dp_size = new_sz;
}

/* Recursive computation with memoisation */
static seq_t compute(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (seen[n]) return dp[n];
    ensure_spf(n);
    seq_t sum = (seq_t)0;
    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d == 0) {
            if (is_composite(d)) sum += compute(n / d);
            idx_t e = n / d;
            if (e != d && is_composite(e)) sum += compute(n / e);
        }
    }
    if (is_composite(n)) sum += compute(1);  /* trivial factorisation */
    dp[n] = sum;
    seen[n] = 1;
    return sum;
}

seq_t a(idx_t n)
{
    ensure_dp(n);
    if (!seen[1]) { dp[1] = (seq_t)1; seen[1] = 1; }
    return compute(n);
}
