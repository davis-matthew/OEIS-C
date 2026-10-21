/*
 * A066153  (product of digits of n) * (sum of digits of n) = 4n.
 * https://oeis.org/A066153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A066153 – find n such that (product of digits of n) * (sum of digits of n) = 4n
   The function scans sequentially, memoising found terms, so it works for any sequence index
   starting at offset 1 (a(1)=0). */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoisation storage */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_test = 0;  /* next candidate integer to test */

/* append value to cache, growing capacity as needed */
static void cache_append(seq_t val) {
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *new_ptr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();            /* out of memory */
        cache = new_ptr;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* compute a(n) */
seq_t a(idx_t n) {
    /* ensure we have at least n terms */
    while (cache_len < n) {
        idx_t x = next_test++;
        seq_t prod = 1;
        seq_t sum  = 0;
        if (x == 0) {          /* special case: 0 */
            prod = 0;
            sum  = 0;
        } else {
            idx_t y = x;
            while (y > 0) {
                unsigned digit = y % 10;
                prod *= (seq_t)digit;
                sum  += (seq_t)digit;
                y    /= 10;
            }
        }
        if (prod * sum == (seq_t)4 * (seq_t)x) {
            cache_append((seq_t)x);
        }
    }
    return cache[n-1];   /* offset 1 */
}
