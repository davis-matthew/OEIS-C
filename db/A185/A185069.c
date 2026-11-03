/*
 * A185069  Primes of the form k*6^m +1 with k prime and m an integer.
 * https://oeis.org/A185069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 451, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 931 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes of the form k*6^m + 1 with k prime */
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

/* Static cache of generated terms */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next prime candidate to test; starts with the first odd prime */
static seq_t next_candidate = 3;

/* Test if a prime p satisfies the defining condition */
static bool qualifies(seq_t p)
{
    seq_t q = p - 1;
    while (q % 6 == 0)
        q /= 6;
    return is_prime_big(q);
}

/* Return the n-th term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n<1, return 0 for safety */
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        /* Find the next prime candidate */
        while (!is_prime_big(next_candidate)) {
            if (next_candidate == 3)
                next_candidate = 5;
            else
                next_candidate += 2;
        }

        /* If the prime satisfies the property, store it */
        if (qualifies(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                seq_cache = mem_resize(seq_t, seq_cache, cache_cap);
            }
            seq_cache[cache_len++] = next_candidate;
        }

        /* Advance to the next candidate */
        if (next_candidate == 3)
            next_candidate = 5;
        else
            next_candidate += 2;
    }

    return seq_cache[(size_t)n - 1];
}
