/*
 * A230199  Sum of digits of n-th palindromic prime.
 * https://oeis.org/A230199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = A007953(A002385(n)) – sum of digits of the n‑th
 *            palindromic prime
 * Range:     u8 n <= 10, u16 n <= 587, u32 n <= 5953, u64 n <= 5953,
 *            u128 n <= 5953, big any n
 * Validated: standard, pass, 85 terms through n = 5920 (2026-11-03)
 * Helpers:   digit_sum, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

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
        cache = mem_resize(seq_t, cache, cache_cap);
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
