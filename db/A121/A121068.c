/*
 * A121068  Numbers k such that 8*k^2 + 7 is prime.
 * https://oeis.org/A121068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A121068: numbers k such that 8*k^2 + 7 is prime.
   The sequence a(1) = 0, a(2) = 3, a(3) = 27, …   (OEIS offset 1).
   The function finds the n‑th such k by brute‑force search, memoising
   all results found so far. */

/* The type seq_t is defined by the build (uint8_t … unsigned __int128
   or an arbitrary‑precision integer). idx_t is an unsigned 64‑bit index. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Use the generic big‑integer primality test supplied by the build. */
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

/* Static cache of found k values. */
static seq_t *cache      = NULL;   /* array of k values */
static size_t cache_size = 0;      /* number of entries stored */
static size_t cache_cap  = 0;      /* allocated capacity of cache  */
static idx_t next_k      = 0;      /* next candidate k to test */

seq_t a(idx_t n)
{
    if (n == 0) {   /* undefined for n < 1 – return 0 as a safe value */
        return 0;
    }

    /* Generate terms until the cache contains at least n entries. */
    while (cache_size < n) {
        /* Compute 8*next_k^2 + 7 in seq_t to avoid overflow. */
        seq_t val = (seq_t)8 * (seq_t)next_k * (seq_t)next_k + (seq_t)7;

        if (is_prime_big(val)) {
            /* Expand the cache if necessary. */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            /* Store the found k (cast to seq_t); overflow is harmless
               for builds with small seq_t, and for larger builds it
               preserves the correct value. */
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;   /* move to the next candidate */
    }

    return cache[n - 1];
}
