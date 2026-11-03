/*
 * A221981  Primes q = 4*p+1, where p == 2 (mod 5) is also prime.
 * https://oeis.org/A221981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 88, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 8487 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of primes p \u2260 2 (mod 5) with q = 4*p+1 prime.
   a(n) = 4*p + 1 where p is a prime ≡ 2 (mod 5) and 4*p + 1 is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility function protos */
#include "../utils/is_prime_seq.h"
#include "../utils/next_prime.h"
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
    if (n == 0) return (seq_t)0;   /* undefined for indices before the offset */

    /* static cache of already found terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    /* last p examined; start with 1 so that next_prime gives 2 */
    static uint64_t last_p    = 1;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* get next prime p */
        uint64_t p = next_prime(last_p);
        last_p = p;

        /* test the special congruence condition */
        if ((p % 5U) == 2U) {
            /* compute q = 4*p + 1 */
            seq_t q = (seq_t)4 * (seq_t)p + (seq_t)1;

            /* q must also be prime */
            if (is_prime_big(q)) {
                /* extend cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();          /* allocation failed */
                    cache     = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = q;
            }
        }
        /* next prime will be found on the next loop iteration */
    }

    return cache[n - 1];
}
