/*
 * A142576  Primes congruent to 46 mod 53.
 * https://oeis.org/A142576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 967 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: nth prime congruent to 46 modulo 53.
   Computed by generating candidates of the form 53*k+46 and testing
   primality with the helper `is_prime_big`. */

/* Required helper */
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

#include <stdbool.h>
#include <stdlib.h>

/* Static cache for computed primes */
static seq_t *cache = NULL;          /* array of primes */
static idx_t cache_size = 0;         /* number of primes stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */
static idx_t next_k = 5;             /* 53*next_k + 46 is the next candidate */

/* Return the nth element of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Offset is 1; behaviour for n < 1 is irrelevant. */
    if (n == 0)
        return 0;

    /* Expand the cache until it contains at least n primes. */
    while (cache_size < n)
    {
        /* Candidate prime of the form 53*k + 46 */
        seq_t cand = (seq_t)53 * (seq_t)next_k + (seq_t)46;

        /* Test primality using the provided helper. */
        if (is_prime_big(cand))
        {
            /* Grow the cache if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    exit(1);   /* Out of memory – terminate. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = cand;   /* store the found prime */
        }
        ++next_k;   /* advance to the next candidate */
    }

    /* Sequence is 1‑based. */
    return cache[n - 1];
}
