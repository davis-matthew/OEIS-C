/*
 * A158746  Numbers n such that 30*n + 13 is prime.
 * https://oeis.org/A158746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: linear scan – for each non‑negative integer n, test whether
   30*n+13 is prime.  Store the qualifying n’s in a cache and return
   the n‑th element of that cache.  The function works for both
   unsigned integer and arbitrary‑precision seq_t types. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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

seq_t a(idx_t n)
{
    /* static cache for previously computed elements */
    static seq_t *cache = NULL;          /* array of found n's          */
    static size_t cache_size = 0;        /* number of elements cached   */
    static size_t cache_cap  = 0;        /* capacity of the cache array */
    static idx_t next_candidate = 0;     /* next n to test for primality */

    /* Guard against invalid indices (not expected per the spec). */
    if (n == 0) return 0;

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Compute candidate value: 30 * next_candidate + 13. */
        seq_t val = (seq_t)30 * (seq_t)next_candidate + 13;

        /* Test primality. */
        if (is_prime_big(val)) {
            /* Ensure enough room in the cache. */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();       /* allocation failed – terminate */
                cache = tmp;
                cache_cap = new_cap;
            }
            /* Store the qualifying n. */
            cache[cache_size++] = (seq_t)next_candidate;
        }
        /* Move to the next integer. */
        next_candidate++;
    }

    return cache[n - 1];
}
