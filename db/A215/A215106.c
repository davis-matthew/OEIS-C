/*
 * A215106  Primes congruent to {3, 5, 6} mod 11.
 * https://oeis.org/A215106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 69 terms through n = 950 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Table: primes congruent to 3, 5 or 6 modulo 11
   Computation method: incremental search using is_prime_big.
   The first terms are 3, 5, 17, 47, …
*/

#include <stdbool.h>
#include <stdlib.h>
/* retrieves the function bool is_prime_big(seq_t); */
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
    /* static state: cache of found primes and next candidate to test */
    static seq_t *cache   = NULL;   /* array of primes found so far */
    static size_t cache_cap = 0;    /* capacity of cache */
    static size_t cache_size = 0;   /* number of primes stored */
    static seq_t next_candidate = (seq_t)3;  /* next odd number to test */
    static bool initialized = false;        /* flag for one‑time init */

    /* initialise only once */
    if (!initialized) {
        /* first prime in the sequence is 3 */
        initialized = true;
    }

    /* generate primes until we have at least n of them */
    while (cache_size < n) {
        seq_t mod = next_candidate % (seq_t)11;   /* remainder modulo 11 */

        if (mod == (seq_t)3 || mod == (seq_t)5 || mod == (seq_t)6) {
            if (is_prime_big(next_candidate)) {
                /* enlarge cache if necessary */
                if (cache_size == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) abort();                /* out of memory */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = next_candidate;
            }
        }

        next_candidate += (seq_t)2;   /* skip even numbers */
    }

    return cache[n - 1];
}
