/*
 * A087545  a(0) = 0, a(1) = 1, a(n) = smallest multiple of n beginning with
 *          the sum of two previous terms.
 * https://oeis.org/A087545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 1,
   a(n) = smallest multiple of n whose decimal representation starts
   with the decimal digits of a(n‑1)+a(n‑2). */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoized sequence terms */
static idx_t cache_cap = 0;       /* allocated size of cache */
static idx_t max_computed = 0;    /* first index not yet computed */

/* Helper: grow cache to at least target capacity */
static void grow_cache(idx_t target) {
    if (target < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 1;
    while (new_cap <= target) new_cap <<= 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* Ensure cache capacity */
    grow_cache(n + 1);

    /* Compute missing terms up to n */
    for (idx_t i = max_computed; i <= n; ++i) {
        if (i == 0) {
            cache[0] = 0;
            continue;
        }
        if (i == 1) {
            cache[1] = 1;
            continue;
        }

        /* a(i) = smallest multiple of i starting with a(i-1)+a(i-2) */
        seq_t sum = cache[i - 1] + cache[i - 2];
        idx_t sum_mod = (idx_t)(sum % (seq_t)i);  /* reduce modulo i */

        seq_t pow10_seq = 1;
        idx_t pow10_mod = 1 % i;
        for (;;) {
            idx_t r = (sum_mod * pow10_mod) % i;
            idx_t t = (i - r) % i;                /* candidate remainder correction */

            /* t < pow10_seq ? */
            if ((seq_t)t < pow10_seq) {
                cache[i] = sum * pow10_seq + (seq_t)t;
                break;
            }

            /* next power of 10 */
            pow10_mod = (pow10_mod * 10) % i;
            pow10_seq *= 10;
        }
    }

    max_computed = n + 1;
    return cache[n];
}
