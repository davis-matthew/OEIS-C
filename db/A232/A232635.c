/*
 * A232635  Expansion of psi(x) * phi(x^2)^2 in powers of x where phi(), psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A232635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of the period‑8 sequence b(d) =
 *            [1,3,1,-7,1,3,1,-3]. The sequence satisfies a(0)=1 and for n≥1:
 *            a(n) = (1/n) * Σ_{k=1..n} S(k) * a(n-k), where S(k) = Σ_{d|k} d
 *            * b(d)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache   = NULL;
static int64_t *S_cache = NULL;
static size_t cache_cap = 0;
static size_t computed  = 0;
static const int8_t pattern[8] = { 1, 3, 1, -7, 1, 3, 1, -3 };

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(size_t n)
{
    if (cache_cap > n) return;
    size_t new_cap = n + 1;            /* include index 0 */
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    S_cache = (int64_t *)realloc(S_cache, new_cap * sizeof(int64_t));
    if (!a_cache || !S_cache) abort();
    cache_cap = new_cap;
}

/* Compute S(k) for k from computed up to n (inclusive). */
static void compute_S_up_to(size_t n)
{
    for (size_t k = computed; k <= n; ++k) {
        int64_t s = 0;
        for (size_t d = 1; d * d <= k; ++d) {
            if (k % d == 0) {
                int d1 = (int)d;
                int d2 = (int)(k / d);
                s += (int64_t)d1 * pattern[(d1 - 1) & 7];
                if (d1 != d2)
                    s += (int64_t)d2 * pattern[(d2 - 1) & 7];
            }
        }
        S_cache[k] = s;
    }
}

/* Extend the cached a(n) values up to n (inclusive). */
static void extend_to(size_t n)
{
    if (computed > n) return;
    ensure_capacity(n);
    if (computed == 0) {
        a_cache[0] = (seq_t)1;
        computed = 1;
    }
    compute_S_up_to(n);
    for (size_t i = computed; i <= n; ++i) {
        int64_t sum = 0;
        for (size_t k = 1; k <= i; ++k) {
            sum += (int64_t)S_cache[k] * (int64_t)a_cache[i - k];
        }
        a_cache[i] = (seq_t)(sum / (int64_t)i);   /* exact division */
    }
    computed = n + 1;
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    extend_to(idx);
    return a_cache[idx];
}
