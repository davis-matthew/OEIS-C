/*
 * A061881  First (leftmost) digit - second digit + third digit - fourth digit
 *          .... = 12.
 * https://oeis.org/A061881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   The alternating sum of decimal digits of a number (first digit +
 *            second digit – third digit + …) equals 12. a(n) is the nth
 *            positive integer satisfying this property
 * Range:     u8 none, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

// helper: compute alternating sum of digits of n
static int alt_digit_sum(uint64_t n) {
    // extract digits into stack
    unsigned char buf[32];
    int pos = 0;
    while (n) {
        buf[pos++] = n % 10;
        n /= 10;
    }
    // compute sum from most significant digit
    int sum = 0;
    int sign = 1;                     // most significant digit has sign +1
    for (int i = pos - 1; i >= 0; --i) {
        sum += sign * buf[i];
        sign = -sign;
    }
    return sum;
}

// cache the found terms
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 1;

seq_t a(idx_t n) {
    if (n == 0) return 0;                   // undefined for offset 1, but safe
    // grow cache as needed
    while (cache_size < (size_t)n) {
        if (cache_size == cache_cap) {      // allocate / grow
            size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_cache) {
                // allocation failed; abort
                abort();
            }
            cache = new_cache;
            cache_cap = new_cap;
        }
        // find next number satisfying the property
        while (alt_digit_sum(next_candidate) != 12) {
            next_candidate++;
        }
        cache[cache_size++] = (seq_t)next_candidate;
        next_candidate++;
    }
    return cache[n - 1];
}
