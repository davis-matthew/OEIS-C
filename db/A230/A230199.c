/*
 * A230199  Sum of digits of n-th palindromic prime.
 * https://oeis.org/A230199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = A007953(A002385(n)) – sum of digits of the n‑th
 *            palindromic prime
 * Range:     u8 n <= 10, u16 n <= 587, u32 n <= 69, u64 n <= 5953,
 *            u128 n <= 5953, big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/digit_sum.h"

/* Build a decimal palindrome from a prefix.
   If odd==1 then the palindrome has odd length, otherwise even. */
static seq_t make_palindrome(uint64_t prefix, int odd) {
    seq_t pal = (seq_t)prefix;
    uint64_t k = odd ? prefix / 10 : prefix;
    while (k > 0) {
        pal = pal * 10 + (seq_t)(k % 10);
        k /= 10;
    }
    return pal;
}

/* 10^e as uint64_t; e will be small (≤10) for our usage */
static uint64_t pow10_u64(unsigned int e) {
    uint64_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;          /* digit sums of palindromic primes */
    static size_t cache_len = 0;          /* number of elements in cache   */
    static size_t cache_cap = 0;          /* current cache capacity        */
    static uint64_t cur_len = 1;          /* current palindrome length     */
    static uint64_t cur_prefix_start = 1, cur_prefix_end = 9; /* current prefix range */

    /* 0‑based index into cache */
    const idx_t target = n - 1;

    /* Ensure cache capacity for target index */
    while ((size_t)target >= cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 64;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Generate palindromic primes until we have enough */
    while (cache_len <= (size_t)target) {
        /* If current prefix exhausted, move to next palindrome length */
        if (cur_prefix_start > cur_prefix_end) {
            cur_len++;
            cur_prefix_start = pow10_u64((cur_len + 1) / 2 - 1);
            cur_prefix_end   = pow10_u64((cur_len + 1) / 2) - 1;
        }

        /* Test current palindrome */
        seq_t p = make_palindrome(cur_prefix_start, (int)(cur_len % 2));
        if (is_prime_big(p)) {
            cache[cache_len++] = digit_sum(p);
        }
        cur_prefix_start++;
    }

    return cache[(size_t)target];
}
