/*
 * A122785  Nonprimes m such that 8^m == 8 (mod m).
 * https://oeis.org/A122785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of nonprimes m such that 8^m == 8 (mod m)
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime, mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_m = 1;   /* next integer m to test */

    /* Return cached value if already computed */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Ensure cache has enough capacity for n elements */
    size_t target = (size_t)n;
    if (cache_cap < target) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < target)
            new_cap *= 2;
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (cache) {
            mem_copy(new_cache, cache, cache_len * sizeof(seq_t));
            mem_free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate sequence terms up to the requested index */
    while (cache_len < target) {
        bool ok = false;

        if (next_m == 1) {
            ok = true;                          /* 1 is the first term */
        } else if (!is_prime((idx_t)next_m)) {
            seq_t mod_res = powmod((seq_t)8, (idx_t)next_m, (seq_t)next_m);
            seq_t mod_8 = ((seq_t)8) % (seq_t)next_m;
            ok = (mod_res == mod_8);
        }

        if (ok) {
            cache[cache_len] = (seq_t)next_m;
            cache_len++;
        }
        next_m++;
    }

    return cache[(size_t)n - 1];
}
