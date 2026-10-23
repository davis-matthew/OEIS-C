/*
 * A021999  Decimal expansion of 1/995.
 * https://oeis.org/A021999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the nth decimal digit of 1/995, offset 0
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_size = 0;
static uint32_t rem = 1;   /* remainder of the long division 1/995 */

/* ensure the cache can hold at least 'needed' elements */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_capacity)
        return;
    idx_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) {
        abort();           /* allocation failure – shouldn't happen in tests */
    }
    cache = new_ptr;
    cache_capacity = new_cap;
}

seq_t a(idx_t n) {
    if (n < cache_size)
        return cache[n];

    ensure_capacity(n + 1);

    /* generate missing digits up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        unsigned long long temp = (unsigned long long)rem * 10ULL;
        seq_t digit = (seq_t)(temp / 995ULL);
        rem = (uint32_t)(temp % 995ULL);
        cache[i] = digit;
    }
    cache_size = n + 1;
    return cache[n];
}
