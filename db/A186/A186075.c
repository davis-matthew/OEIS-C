/*
 * A186075  Irregular triangle, read by rows, of primes with suffix n and
 *          digits "9" prepended, otherwise 0.
 * https://oeis.org/A186075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 31, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   digits_of, is_prime_big, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A186075: 
 * For each row number r (starting at 1) generate numbers 
 *   L(r,k) = (repunit_k)*10^d + r          where d = digits(r) and repunit_k = 99…9 (k digits).
 * Emit all L(r,k) that are prime, stopping at the first composite L(r,k).  
 * If a row produces no primes, output a single 0 for that row.  
 * The sequence is read by rows, concatenating the outputs of all rows.
 */

#include <stdbool.h>

#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t); */
#include "../utils/digits_of.h"     /* int digits_of(idx_t); */
#include "../utils/seq_alloc_new_array.h"   /* seq_t *seq_alloc_new_array(size_t); */
#include "../utils/seq_realloc.h"            /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t next_row = 1;           /* row number to be processed next */

    /* Grow the cache when needed */
    while (cache_len < n) {
        idx_t r = next_row++;
        /* Compute base = 10^digits(r) */
        int d = digits_of(r);
        seq_t base = (seq_t)1;
        for (int i = 0; i < d; ++i) base *= (seq_t)10;

        /* repunit = 9, then repeat: repunit = repunit*10 + 9 */
        seq_t repunit = (seq_t)9;
        seq_t first = repunit * base + (seq_t)r;

        bool any_prime = false;
        if (is_prime_big(first)) {
            /* Ensure capacity */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = first;
            any_prime = true;
        } else {
            /* No prime in this row -> output a single 0 */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)0;
            continue;   /* move to next row */
        }

        /* Continue for k >= 2 until first composite */
        while (true) {
            repunit = repunit * (seq_t)10 + (seq_t)9;
            seq_t val = repunit * base + (seq_t)r;
            if (is_prime_big(val)) {
                if (cache_len + 1 > cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = val;
                any_prime = true;
                continue;
            } else {
                /* Stop at first composite; do not output zero for it */
                break;
            }
        }
        /* If the row produced primes, we do not output an extra zero. */
    }

    return cache[n - 1];
}
