/*
 * A159047  Primes which are triangular numbers plus 3.
 * https://oeis.org/A159047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 33, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 970 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A159047
    Formula: a(n) is the nth prime of the form T(k)+3 where T(k)=k(k+1)/2.
    The function searches successive triangular numbers, tests each
    T(k)+3 for primality with is_prime_big, and caches the results. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
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

static size_t  cache_count = 0;          /* number of primes stored */
static size_t  cache_cap   = 0;          /* current capacity of cache */
static seq_t  *cache_primes = NULL;     /* cached primes */
static idx_t  next_k = 0;                /* next triangular index to test */

seq_t a(idx_t n)
{
    /* Gather primes until we have at least n of them */
    while (cache_count < (size_t)n)
    {
        idx_t k = next_k++;                             /* triangular index */
        seq_t tri = ((seq_t)k) * (((seq_t)k) + 1) / 2;   /* T(k) in seq_t   */
        seq_t cand = tri + 3;                           /* candidate prime */

        if (is_prime_big(cand))
        {
            /* Grow cache when needed */
            if (cache_count >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                seq_t *tmp = mem_resize(seq_t, cache_primes, new_cap);
                if (!tmp) abort();                      /* allocation failed */
                cache_primes = tmp;
                cache_cap   = new_cap;
            }
            cache_primes[cache_count++] = cand;          /* store new prime   */
        }
    }

    /* Return the (n-1)-th stored prime (0‑indexed array) */
    return cache_primes[(size_t)n - 1];
}
