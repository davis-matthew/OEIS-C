/*
 * A084979  Palindromes such that the product of the digits + 1 is prime.
 * https://oeis.org/A084979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 225, u32 n <= 538, u64 n <= 8696,
 *            u128 n <= 8696, big any n
 * Helpers:   digit_product, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084979:
   Palindromes such that the product of the digits + 1 is prime.
   Computation is performed by generating palindromes in increasing
   numeric order and testing the required condition.  Results are
   cached so that subsequent calls are efficient regardless of call
   order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/is_prime_big.h"

/* Helper: power of ten as idx_t */
static idx_t pow10_int(idx_t exp)
{
    idx_t r = 1;
    for (; exp > 0; --exp) {
        r *= 10;
    }
    return r;
}

/* Build a palindrome of given length from the half value. */
static seq_t make_pal(idx_t half, idx_t len)
{
    uint8_t buf[20];
    idx_t m = half;
    size_t dlen = 0;
    /* Store digits least‑to‑most in buf */
    do {
        buf[dlen++] = m % 10;
        m /= 10;
    } while (m);

    seq_t val = 0;
    /* High side: most significant digits */
    for (size_t k = dlen; k-- > 0; ) {
        val = val * 10 + buf[k];
    }
    /* Low side
       For odd length skip middle digit (buf[0]),
       for even length include all digits. */
    size_t start = (len & 1) ? 1 : 0;
    for (size_t j = start; j < dlen; ++j) {
        val = val * 10 + buf[j];
    }
    return val;
}

/* External function: a(n) */
seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_size = 0;

    /* State for generating palindromes */
    static idx_t cur_len = 1;
    static idx_t cur_half_start = 0, cur_half_end = 0;
    static idx_t cur_half = 0;

    /* Initialise generators on first call */
    if (cache_size == 0) {
        /* Cache empty, initialise generator */
        cur_len = 1;
        cur_half_start = 1;
        cur_half_end = 9;
        cur_half = cur_half_start;
    }

    /* Generate terms until we reach n */
    while (cache_size < n) {
        /* Build next palindrome */
        seq_t pal = make_pal(cur_half, cur_len);

        /* Test condition: product of digits + 1 is prime */
        seq_t prod = digit_product(pal);
        seq_t cand = prod + 1;
        if (is_prime_big(cand)) {
            /* Store in cache */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = pal;
        }

        /* Advance half index */
        ++cur_half;
        if (cur_half > cur_half_end) {
            /* Move to next palindrome length */
            ++cur_len;
            idx_t half_len = (cur_len + 1) / 2;
            cur_half_start = (half_len == 1) ? 1 : pow10_int(half_len - 1);
            cur_half_end = pow10_int(half_len) - 1;
            cur_half = cur_half_start;
        }
    }

    return cache[n - 1];
}
