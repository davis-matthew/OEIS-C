/*
 * A120229  Split-floor-multiplier sequence (SFMS) using multipliers 1/3 and
 *          3. The SFMS using multipliers r and s is here introduced: for
 *          every positive integer n and positive real number r, let [rn]
 *          abbreviate floor(rn). Then SFMS(r, s), where max {r, s} > 1, is
 *          the sequence a defined by a(n)=[rn] if [rn]...
 * https://oeis.org/A120229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = floor(n/3) if positive and not yet used; otherwise 3*n. */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static bool *used = NULL;
    static size_t used_cap = 0;
    static idx_t computed = 0;

    if (n == 0) { /* undefined, but return 0 for safety */
        return 0;
    }

    /* Ensure enough space for cache up to n */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Ensure used array can index values up to 3*n */
    size_t need_used = (size_t)(3 * n) + 1;
    if (used_cap < need_used) {
        size_t new_cap = used_cap ? used_cap << 1 : 16;
        if (new_cap < need_used) new_cap = need_used;
        bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used = tmp;
        used_cap = new_cap;
    }

    /* Compute terms from (computed+1) up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t candidate = i / 3;
        seq_t val;
        if (candidate > 0 && !used[candidate]) {
            val = (seq_t)candidate;
            used[candidate] = true;
        } else {
            val = (seq_t)i * 3;
            used[(idx_t)val] = true;
        }
        cache[i - 1] = val;
    }
    computed = n;
    return cache[n - 1];
}
