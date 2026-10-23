/*
 * A043804  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 6 mod 8.
 * https://oeis.org/A043804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th integer whose base‑3 representation has a
   number of runs congruent to 6 modulo 8. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static seq_t last_checked = 0;

static inline bool has_property(seq_t n) {
    int runs = 0;
    int prev = -1;
    seq_t tmp = n;
    while (tmp > 0) {
        int digit = (int)(tmp % 3);
        if (digit != prev) {
            ++runs;
            prev = digit;
        }
        tmp /= 3;
    }
    if (runs == 0) runs = 1; /* for n == 0, which is not used */
    return (runs & 7) == 6;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1 */
    while (cache_size < n) {
        seq_t num = last_checked + 1;
        last_checked = num;
        if (has_property(num)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap << 1;
                seq_t *new_cache = (seq_t *)realloc(cache,
                                                     new_cap * sizeof(seq_t));
                if (!new_cache) return 0; /* allocation failure */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_size++] = num;
        }
    }
    return cache[n - 1];
}
