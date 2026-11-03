/*
 * A142844  Primes congruent to 46 mod 61.
 * https://oeis.org/A142844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 109, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 999 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the primes congruent to 46 modulo 61.
   a(n) = the n‑th prime p such that p ≡ 46 (mod 61).  */

/* OEIS link: A142844  Primes congruent to 46 mod 61 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
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

/* Cache for previously computed terms (zero‑based).  */
static seq_t *cache = NULL;
static size_t cache_count = 0;   /* number of terms stored in cache */
static size_t cache_cap   = 0;   /* allocated capacity of cache */
static idx_t  last_k      = 0;   /* last k examined (k such that p = 46 + 61*k) */

/* Return the n-th term, where the sequence is 1‑based.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 0 */

    /* Return a cached value if it already exists. */
    if (cache != NULL && (size_t)n <= cache_count)
        return cache[n - 1];

    /* Grow the cache if it doesn't exist or is too small.  */
    if (cache == NULL) {
        cache_cap   = (size_t)(n > 128 ? n : 128);
        cache       = mem_new(seq_t, cache_cap);
        cache_count = 0;
        last_k      = 0;
    } else if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = (size_t)n;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL)
            abort();   /* out of memory */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Generate missing terms.  */
    while (cache_count < (size_t)n) {
        ++last_k;                                   /* next candidate k */

        /* Compute p = 46 + 61*k in a type that can hold it.  */
        uint64_t p64 = 46ULL + 61ULL * (uint64_t)last_k;

        /* Cast to seq_t for the primality test.  If seq_t is too small
           to hold p64, the cast will overflow and p will be wrong,
           but such a value will not be needed to return a valid term
           that fits into seq_t.  */
        seq_t p = (seq_t)p64;

        if (is_prime_big(p))
            cache[cache_count++] = p;
    }

    return cache[n - 1];
}
