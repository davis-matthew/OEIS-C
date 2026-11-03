/*
 * A175276  Base-4 pandigital primes: primes having at least one of each digit
 *          0,1,2,3, when written in base 4.
 * https://oeis.org/A175276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3701, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9392 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes whose base‑4 representation contains each digit 0,1,2,3 at least once. */
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

/* Static state shared among all calls to a(n). */
static seq_t *cache = NULL;
static size_t  cache_cap  = 0;   /* allocated capacity */
static size_t  cache_len  = 0;   /* number of cached values */
static idx_t   next_check = 2;   /* smallest integer to test next */

/* Return the n‑th base‑4 pandigital prime value. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* OEIS offset starts at 1 */

    /* Grow the cache until we have at least n primes. */
    while (cache_len < (size_t)n)
    {
        /* Use a 64‑bit variable to hold the candidate so that we can examine
           base‑4 digits and test primality without overflow when seq_t is small. */
        uint64_t cand64 = (uint64_t)next_check;

        /* Build a bit mask of the digits appearing in the base‑4
           representation of cand64. */
        unsigned mask = 0;
        uint64_t t = cand64;
        while (t > 0)
        {
            int d = (int)(t % 4);   /* digit 0–3 */
            mask |= 1U << d;
            t    /= 4;
        }

        /* If the mask shows that we have all four digits 0,1,2,3, and
           the number is prime, store it.  We only test primality when the
           value fits in seq_t, otherwise we skip it. */
        if (mask == 15)
        {
            /* Check that cand64 does not overflow seq_t. */
            if (cand64 <= (uint64_t)(seq_t)-1)
            {
                seq_t cand_small = (seq_t)cand64;
                if (is_prime_big(cand_small))
                {
                    /* Grow the cache if necessary. */
                    if (cache_len == cache_cap)
                    {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                        if (!tmp) return 0;   /* allocation failure */
                        cache      = tmp;
                        cache_cap  = new_cap;
                    }
                    cache[cache_len++] = cand_small;
                }
            }
        }

        ++next_check;
    }

    /* Return the cached value.  Index n is 1‑based. */
    return cache[n - 1];
}
