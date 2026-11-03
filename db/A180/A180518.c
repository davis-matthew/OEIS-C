/*
 * A180518  Primes that become a different prime under the mapping 0 <=> 3.
 * https://oeis.org/A180518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime satisfying the property:
   after swapping all decimal digits 0 and 3 the resulting number is a
   different prime.  The implementation builds the sequence on demand
   and caches the found values.  It works for any seq_t (fixed‑width
   or arbitrary precision) and any idx_t (uint64_t). */
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
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

static seq_t map_digits(seq_t x)
{
    seq_t res = 0;
    seq_t pow10 = 1;
    while (x > 0) {
        int d = (int)(x % 10);
        int sd = (d == 0) ? 3 : (d == 3) ? 0 : d;
        res += (seq_t)sd * pow10;
        pow10 *= 10;
        x /= 10;
    }
    return res;
}

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;
/* Current candidate for prime testing, kept as seq_t for arbitrary‑precision */
static seq_t next_candidate = 2;

seq_t a(idx_t n)
{
    if (n == 0)          /* no definition for n=0; return 0 as sentinel */
        return 0;

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Jump to next prime candidate */
        while (!is_prime_big(next_candidate))
            next_candidate++;

        /* Check mapping property */
        seq_t mapped = map_digits(next_candidate);
        if (mapped != next_candidate && is_prime_big(mapped)) {
            /* Ensure cache has space */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                /* realloc with explicit cast for C++ compatibility */
                cache = mem_resize(seq_t, cache, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }

        /* Advance to the next integer for the next iteration */
        next_candidate++;
    }

    return cache[n - 1];
}
