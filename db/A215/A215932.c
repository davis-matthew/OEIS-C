/*
 * A215932  Happy reversible primes.
 * https://oeis.org/A215932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 43, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   get_digits, is_prime_u64, mem, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of those primes that are happy and whose
 * decimal reversal is also a happy prime.  To generate a(n) we
 * enumerate primes in increasing order, test happiness for the
 * prime and for its reverse, and store the qualifying primes in
 * a cache.  The function is memoised, so any permutation of
 * calls to a(n) gives correct results quickly.  The algorithm
 * requires only primality testing and digit extraction – both
 * provided by the utility functions. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/get_digits.h"
#include "../utils/reverse_digits.h"
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

static inline bool is_happy(seq_t x)
{
    /* Determine happiness by iterating sums of digit squares. */
    while (x != ((seq_t)1) && x != ((seq_t)4)) {
        uint64_t sum = 0;
        uint8_t buf[32];
        size_t len;
        get_digits(x, buf, &len);
        for (size_t i = 0; i < len; ++i) {
            uint64_t d = buf[i];
            sum += d * d;
        }
        x = (seq_t)sum;
    }
    return x == ((seq_t)1);
}

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache      = NULL;
    static size_t  cache_len = 0;
    static size_t  cache_cap = 0;
    /* Current candidate to test */
    static idx_t   candidate  = 2;
    static bool    started    = false;

    /* Initialise cache on first call */
    if (!started) {
        cache_cap = 64;
        cache     = mem_new(seq_t, cache_cap);
        started   = true;
    }

    /* Fill cache until we have at least n elements */
    while (cache_len < n) {
        /* Test primality */
        if (!is_prime_big((seq_t)candidate)) {
            candidate++;
            continue;
        }

        /* Test happiness of the prime itself */
        if (is_happy((seq_t)candidate)) {
            /* Reverse the number */
            seq_t rev = reverse_digits((seq_t)candidate);
            /* Test primality and happiness of the reverse */
            if (is_prime_big(rev) && is_happy(rev)) {
                /* Ensure cache capacity */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *new_arr = mem_resize(seq_t, cache, new_cap);
                    if (!new_arr) { /* allocation failed; abort gracefully */
                        mem_free(cache);
                        cache = NULL;
                        return (seq_t)0;
                    }
                    cache     = new_arr;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)candidate;
            }
        }
        candidate++;
    }

    return cache[n - 1];
}
