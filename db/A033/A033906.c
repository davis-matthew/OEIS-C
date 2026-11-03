/*
 * A033906  Sort then Add, a(1)=20.
 * https://oeis.org/A033906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + sorted_digits(a(n-1)) with a(1) = 20, a(2) = 22
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

static seq_t sorted_digits(seq_t x)
{
    unsigned cnt[10] = {0};
    while (x != (seq_t)0) {
        unsigned d = (unsigned)(x % (seq_t)10);
        ++cnt[d];
        x /= (seq_t)10;
    }
    seq_t res = (seq_t)0;
    for (unsigned d = 0; d < 10; ++d)
        for (unsigned c = 0; c < cnt[d]; ++c)
            res = res * (seq_t)10 + (seq_t)d;
    return res;
}

seq_t a(idx_t n)
{
    /* static cache storing a(1)...a(last_computed) */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated size (including index 0) */
    static idx_t last_computed = 0; /* greatest n that has been computed */

    if (n == 0) return (seq_t)0;    /* safety guard */

    if (cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1 < 3 ? 3 : n + 1);   /* cache[1], cache[2] are set below */
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (!new_cache) return (seq_t)0; /* allocation failure */

        /* copy existing data if any */
        if (cache)
            for (idx_t i = 0; i <= last_computed; ++i)
                new_cache[i] = cache[i];

        new_cache[0] = (seq_t)0; /* unused element */

        if (cache) free_seq_array(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* initialize first two terms if not yet done */
    if (last_computed < 2) {
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)22;
        last_computed = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        cache[i] = prev + sorted_digits(prev);
    }
    last_computed = n;

    return cache[n];
}
