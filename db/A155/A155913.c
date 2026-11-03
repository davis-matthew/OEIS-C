/*
 * A155913  Primes p such that (sum of digits of p) - (last digit of p) is
 *          prime.
 * https://oeis.org/A155913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) returns the nth prime p such that (sum of digits of p) −
 *            (last digit of p) is prime
 * Range:     u8 n <= 18, u16 n <= 2245, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9673 (2026-11-03)
 * Helpers:   digit_sum, is_prime, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence starts 23, 29, 31, 37, 53, … (offset 1).  */
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime.h"
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

/* Static cache of result terms.  */
static seq_t *seq_cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* The next integer to test when searching for the sequence. */
static idx_t candidate = 2;

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t value)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
        if (!tmp) abort();          /* memory allocation failed */
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_size++] = value;
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1; n==0 is outside of definition */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n) {
        /* Find the next prime satisfying the property. */
        for (;;) {
            if (is_prime(candidate)) {
                seq_t val   = candidate;            /* convert to seq_t for digit_sum */
                seq_t sum   = digit_sum(val);
                seq_t last  = candidate % 10;
                seq_t diff  = sum - last;           /* sum ≥ last, diff is non‑negative */

                if (is_prime_big(diff)) {           /* diff must be a prime */
                    cache_append(candidate);
                    break;
                }
            }
            ++candidate;   /* continue with next integer */
        }
        ++candidate;   /* start searching from the next integer after the found prime */
    }

    return seq_cache[(size_t)n - 1];
}
