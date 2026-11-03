/*
 * A138624  Nonnegative integers k such that 17*k+5 is prime.
 * https://oeis.org/A138624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 567, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9819 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonnegative integers k such that 17*k+5 is prime.  The function
   a(idx_t n) returns the (n‑th) element of this sequence with
   offset 1.  It uses incremental search, testing each candidate
   with the provided is_prime_big() routine and memoises the
   results so that calls in any order are efficient. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

/* Cache of already found terms.
   cache_len   – number of terms stored
   cache_cap   – allocated capacity
   next_k      – smallest k not yet examined        */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static seq_t next_k = 0;

seq_t a(idx_t n)
{
    if (n == 0)       /* behaviour below offset is irrelevant */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Test candidate next_k : 17*next_k + 5 */
        seq_t val = (seq_t)17 * next_k + (seq_t)5;
        if (is_prime_big(val)) {
            /* add next_k to cache */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();              /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_k;
        }
        next_k++;
    }

    return cache[n - 1];
}
