/*
 * A157043  Primes p such that 6p-7, 6p-5, 6p-3, 6p-1 are not prime.
 * https://oeis.org/A157043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 551, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 65 terms through n = 910 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes p such that 6p-7, 6p-5, 6p-3, 6p-1 are all composite.
   a(n) is the n‑th such prime (offset 1).  The function builds a
   static cache of found terms, so any order of calls works.   */

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

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache      = NULL;
    static idx_t  cache_size = 0; /* number of terms stored */
    static idx_t  cache_cap  = 0; /* allocated capacity     */

    /* next candidate to test for primality */
    static seq_t candidate = 3; /* first odd prime */

    /* initialise cache if necessary */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(EXIT_FAILURE);
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* find next prime */
        while (!is_prime_big(candidate))
            candidate += 2;          /* skip even numbers */

        seq_t p = candidate;          /* p is prime */

        /* advance candidate for next search */
        candidate += 2;

        /* check the four numbers that must be composite */
        seq_t v1 = p * 6 - 7;
        seq_t v2 = p * 6 - 5;
        seq_t v3 = p * 6 - 3;
        seq_t v4 = p * 6 - 1;

        if (!is_prime_big(v1) && !is_prime_big(v2) &&
            !is_prime_big(v3) && !is_prime_big(v4)) {
            /* ensure enough capacity */
            if (cache_size >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_size++] = p;
        }
    }

    return cache[n - 1];
}
