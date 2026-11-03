/*
 * A023231  Primes p such that 8*p + 7 is also prime.
 * https://oeis.org/A023231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 223, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 835 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p for which 8p+7 is also prime.
   It generates primes in increasing order, tests the condition, and
   memoises the qualifying terms.  The implementation works with any
   seq_t/idx_t defined by the build system. */

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

/* Cache of the sequence values */
static seq_t *term_cache = NULL;    /* array of found terms  */
static size_t  cache_len  = 0;     /* number of cached terms */
static size_t  cache_cap  = 0;     /* capacity of cache       */

/* Ensure the cache can hold at least 'needed' elements */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    term_cache = mem_resize(seq_t, term_cache, new_cap);
    cache_cap = new_cap;
}

/* Append a new qualifying prime to the cache */
static void add_term(const seq_t *p)
{
    grow_cache(cache_len + 1);
    term_cache[cache_len++] = *p;
}

/* State used on successive calls */
static seq_t  next_candidate;   /* candidate primes to test    */
static bool    init_next_candidate = false;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1 */

    /* Initialise the candidate counter on first use */
    if (!init_next_candidate) {
        next_candidate = (seq_t)2;
        init_next_candidate = true;
    }

    /* Generate terms until we have at least 'n' of them */
    while (cache_len < (size_t)n) {
        /* Search for the next prime p */
        bool found = false;
        while (!found) {
            if (is_prime_big(next_candidate)) {
                /* Test 8*p+7 */
                seq_t test = next_candidate * (seq_t)8 + (seq_t)7;
                if (is_prime_big(test)) {
                    add_term(&next_candidate);
                    found = true;
                }
            }
            /* Next integer */
            next_candidate += (seq_t)1;
        }
    }

    return term_cache[(size_t)n - 1];
}
