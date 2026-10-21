/*
 * A082158  Number of deterministic completely defined acyclic automata with 3
 *          inputs and n transient labeled states (and a unique absorbing
 *          state).
 * https://oeis.org/A082158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=0..n-1} binomial(n,i) * (-1)^{n-i-1} * *
 *            (i+1)^{3(n-i)} * a(i) ; a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

/* fast exponentiation for seq_t */
static seq_t ipow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur    = base;
    while (exp) {
        if (exp & 1) result = result * cur;
        cur = cur * cur;
        exp >>= 1;
    }
    return result;
}

/* memoisation cache: cache[n] = a(n) */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void ensure_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();          /* allocation failed */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (cache_len == 0) {          /* first call: initialise a(0) */
        ensure_cache(1);
        cache[0] = 1;
        cache_len = 1;
    }
    if (n < cache_len) return cache[n];  /* already computed */

    ensure_cache(n + 1);

    for (size_t k = cache_len; k <= n; ++k) {
        if (k == 0) {
            cache[0] = 1;
            continue;
        }
        seq_t sum = 0;                 /* seq_t supports += */
        for (size_t i = 0; i < k; ++i) {
            seq_t bin   = binom(k, i);   /* binomial coefficient */
            seq_t pow   = ipow((seq_t)(i + 1), 3 * (idx_t)(k - i));
            seq_t term  = bin * pow * cache[i];
            /* apply the sign (-1)^{k-i-1} */
            if (((k - i - 1) & 1u) == 1u) term = -term;
            sum += term;
        }
        cache[k] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}
