/*
 * A118360  Start with 1; repeatedly reverse the digits when the number is
 *          written in binary and add 2 to get the next term.
 * https://oeis.org/A118360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A118360: Start with 1; repeatedly reverse the binary
   digits of the preceding term and add 2 (decimal) to get the next. */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"

/* Helper: number of significant bits in a sequence value. */
static unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len;
}

/* Reverse the binary digits of x, using only its significant bits. */
static seq_t reverse_bin(seq_t x)
{
    unsigned len = bitlen_seq_t(x);
    seq_t rev = 0;

    for (unsigned i = 0; i < len; ++i) {
        rev = (rev << 1) | ((x >> i) & 1);
    }
    return rev;
}

/* Cache to support arbitrary access order. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_n = 0;

/* Return the nth term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, just for safety */

    /* Allocate initial cache if necessary. */
    if (cache_n == 0) {
        cache_cap = (n > 16) ? n : 16;
        cache = seq_alloc_array(cache_cap);
        cache[0] = (seq_t)1;      /* a(1) = 1 */
        cache_n = 1;
    }

    /* Grow the cache to hold up to n. */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap < (size_t)n) new_cap *= 2;
        seq_t *tmp = seq_alloc_array(new_cap);
        for (size_t i = 0; i < cache_n; ++i) tmp[i] = cache[i];
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n. */
    while (cache_n < n) {
        seq_t prev = cache[cache_n - 1];
        seq_t rev  = reverse_bin(prev);
        seq_t next = rev + (seq_t)2;   /* add 2 (decimal) */
        cache[cache_n] = next;
        ++cache_n;
    }

    return cache[n - 1];
}
