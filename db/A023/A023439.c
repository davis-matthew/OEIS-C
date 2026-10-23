/*
 * A023439  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-9).
 * https://oeis.org/A023439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 95, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5,
   a(6)=8, a(7)=13, a(8)=21,
   a(n) = a(n-1) + a(n-2) - a(n-9)   for n >= 9
   (OEIS A023439) */
#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;

/* grow cache to hold at least needed elements */
static void ensure_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = (seq_t *)::operator new[](new_cap * sizeof(seq_t));
    /* Copy existing elements */
    for (idx_t i = 0; i < cache_len; ++i) {
        new (&new_cache[i]) seq_t(cache[i]);          /* copy‑construct */
    }
    /* Default‑construct the rest */
    for (idx_t i = cache_len; i < new_cap; ++i) {
        new (&new_cache[i]) seq_t();                  /* default‑construct */
    }
    /* Destroy old cache elements */
    for (idx_t i = 0; i < cache_len; ++i) {
        cache[i].~seq_t();
    }
    ::operator delete[](cache);
#else
    new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Return value if already memoised */
    if (n < cache_len) return cache[n];

    /* Grow cache to accommodate new entries */
    ensure_capacity(n + 1);

    /* Base values for indices 0 … 8 */
    static const seq_t base[9] = {
        /* a(0) … a(8) */
        (seq_t)0, (seq_t)1, (seq_t)1, (seq_t)2,
        (seq_t)3, (seq_t)5, (seq_t)8, (seq_t)13,
        (seq_t)21
    };

    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 9) {
            val = base[i];
        } else {
#ifdef __cplusplus
            /* In C++ use explicit copy if needed */
            val = cache[i - 1] + cache[i - 2] - cache[i - 9];
#else
            val = cache[i - 1] + cache[i - 2] - cache[i - 9];
#endif
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
