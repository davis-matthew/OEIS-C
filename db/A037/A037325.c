/*
 * A037325  Numbers whose base-5 and base-9 expansions have the same digit
 *          sum.
 * https://oeis.org/A037325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑5 digit sum equals
 *            its base‑9 digit sum
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline seq_t sum_digits_base(seq_t x, int base)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % base;
        x /= base;
    }
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* grow cache until it contains at least n elements */
    while (cache_len < n) {
        /* determine next candidate value */
        seq_t candidate = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;

        /* search for the next number satisfying the digit‑sum condition */
        while (1) {
            seq_t s5  = sum_digits_base(candidate, 5);
            seq_t s9  = sum_digits_base(candidate, 9);
            if (s5 == s9) break;
            candidate += 1;
        }

        /* store the found candidate in the cache */
        if (cache_len == cache_cap) {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) {
                /* abort on allocation failure */
                abort();
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_len++] = candidate;
    }

    return cache[n - 1];
}
