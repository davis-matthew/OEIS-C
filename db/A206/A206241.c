/*
 * A206241  a(n) is the smallest number of the form k*a(n-1)+a(n-2) for k>0
 *          that is relatively prime to n, with a(0) = 0 and a(1) = 1.
 * https://oeis.org/A206241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k*a(n-1)+a(n-2) with k > 0 such that
 *            gcd(k*a(n-1)+a(n-2), n) = 1; a(0) = 0, a(1) = 1
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t last_computed = 1; /* cache[0] and cache[1] are set initially */

static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static void ensure_capacity(idx_t need)
{
    if (need < cap) return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap <= (size_t)need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cap = new_cap;
}

static void compute_up_to(idx_t n)
{
    if (cache == NULL) {
        size_t init = 16;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;
        cache[1] = 1;
        cap = init;
        last_computed = 1;
    }
    if (n <= last_computed) return;
    ensure_capacity(n);
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t step = cache[i-1];
        seq_t base = cache[i-2];
        seq_t n_seq = (seq_t)i;
        seq_t candidate;
        idx_t k = 1;
        while (1) {
            candidate = base + step * (seq_t)k;
            idx_t rem = (idx_t)(candidate % n_seq);
            if (gcd_idx(rem, i) == 1) break;
            ++k;
        }
        cache[i] = candidate;
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    compute_up_to(n);
    return cache[n];
}
