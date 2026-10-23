/*
 * A021343  Decimal expansion of 1/339.
 * https://oeis.org/A021343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A021343 - decimal expansion of 1/339
   Formula: a(n) = (floor(10^(n+1) / 339)) % 10
   i.e. digit n of the infinite decimal 0.002949852507...
*/
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;           /* cached digits */
static size_t capacity = 0;           /* current cache capacity */
static idx_t cache_size = 0;          /* number of digits cached */
static uint64_t rem = 1;              /* remainder after last digit */

/* Ensure cache holds at least n+1 digits (indices 0..n). */
static void extend_cache_to(idx_t n) {
    if (n < cache_size) return;

    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
    }

    while (cache_size <= n) {
        if ((size_t)cache_size >= capacity) {
            capacity *= 2;
            cache = (seq_t*)realloc(cache, capacity * sizeof(seq_t));
        }
        uint64_t temp = rem * 10ULL;
        seq_t digit = (seq_t)(temp / 339ULL);
        rem = temp % 339ULL;
        cache[cache_size++] = digit;
    }
}

seq_t a(idx_t n) {
    extend_cache_to(n);
    return cache[n];
}
