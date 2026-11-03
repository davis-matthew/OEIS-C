/*
 * A140374  Primes of the form 26k + 15.
 * https://oeis.org/A140374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes of the form 26k + 15 (OEIS A140374). */
/* The first term corresponds to k = 0 */
#include <stdbool.h>
#include <stdlib.h>
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

/* Prime test for arbitrary‐precision or 64‑bit values. */
/* The function is declared in ../utils/is_prime_big.h. */

seq_t a(idx_t n)
{
    /* Static cache of found primes 26k+15. */
    static seq_t *cache      = NULL;   /* cached values                 */
    static size_t cached_len = 0;      /* number of primes cached       */
    static size_t cache_cap  = 0;      /* capacity of cache array       */
    static idx_t  next_k     = 0;      /* next k to check               */

    /* OEIS offset is 1; return 0 for n==0 (undefined). */
    if (n == 0)
        return (seq_t)0;

    /* Grow cache until we have at least n entries. */
    while (cached_len < (size_t)n)
    {
        /* Candidate prime 26k + 15.  Compute in a larger type first. */
        uint64_t cand64 = 15 + 26 * (uint64_t)next_k;
        next_k++;

        /* Convert to seq_t (possible truncation only when value does not fit). */
        seq_t cand = (seq_t)cand64;

        /* Test primality using the provided helper. */
        if (is_prime_big(cand))
        {
            /* Resize cache if necessary. */
            if (cached_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                    return (seq_t)0;   /* allocation failed */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cached_len++] = cand;
        }
    }

    return cache[n - 1];
}
