/*
 * A142908  Primes congruent to 34 mod 63.
 * https://oeis.org/A142908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 911 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 34 mod 63 (i.e. primes p with p ≡ 34 (mod 63)).
   The sequence is defined as a(n) = the n-th prime of the form 63k + 34,
   for k ≥ 0. The OEIS offset is 1. */

/* Include helper for primality test on generic seq_t values. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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
#include <stdlib.h>
#include <stdint.h>

/* a(n) returns the n-th such prime, where n ≥ 1. */
seq_t a(idx_t n)
{
    /* Static cache of already found primes. */
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t sz      = 0;    /* number of primes stored */
    static idx_t  next_m  = 0;    /* next k value to test */

    /* Max value that can be represented in seq_t. */
    const seq_t max_seq = (seq_t)(-1);               /* unsigned max */
    const unsigned long long max_seq_ul = (unsigned long long)max_seq;

    /* Ensure the cache has initial capacity. */
    if (cap == 0) {
        cap   = 128;
        cache = mem_new(seq_t, cap);
    }

    /* Generate primes until the cache contains at least n entries. */
    while (sz < n) {
        /* Compute candidate 63*next_m + 34 using a wider type to avoid overflow. */
        const unsigned long long cand_ul = 63ULL * (unsigned long long)next_m + 34ULL;

        /* Only consider the candidate if it fits into seq_t. */
        if (cand_ul <= max_seq_ul) {
            const seq_t cand = (seq_t)cand_ul;
            if (is_prime_big(cand)) {
                /* Grow cache if necessary. */
                if (sz == cap) {
                    cap *= 2;
                    cache = mem_resize(seq_t, cache, cap);
                }
                cache[sz++] = cand;
            }
        }
        ++next_m;
    }

    /* Return the (n-1)-th element (since n is 1‑based). */
    return cache[n - 1];
}
