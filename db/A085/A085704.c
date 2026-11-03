/*
 * A085704  Primes of the form prime(k)*2 - prime(k-1), k>1.
 * https://oeis.org/A085704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(k)*2 − prime(k‑1) (k > 1)
 * Range:     u8 n <= 7, u16 n <= 580, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 61 terms through n = 1854 (2026-11-03)
 * Helpers:   is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/next_prime.h"
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* External definition of seq_t and idx_t are provided by the build system. */
seq_t a(idx_t n)
{
    /* Offset is 1; n < 1 is out‑of‑bounds – return 0 just in case. */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Keep the most recent pair of consecutive primes. */
    static uint64_t prev_prime = 2;   /* prime(1) */
    static uint64_t curr_prime = 3;   /* prime(2) */

    /* Grow the cache once before use. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = seq_realloc(NULL, 0, cache_cap);
    }

    /* Generate terms until the n‑th one is available. */
    while (cache_len < n) {
        /* Compute the candidate value 2*curr_prime – prev_prime. */
        uint64_t temp = 2ull * curr_prime - prev_prime;
        if (is_prime_big((seq_t)temp)) {
            /* Append to cache if it is a new term. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)temp;
        }

        /* Advance to the next pair of primes. */
        prev_prime = curr_prime;
        curr_prime = next_prime(curr_prime + 1);
    }

    return cache[n - 1];
}
