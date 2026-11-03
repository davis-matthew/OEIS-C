/*
 * A088632  a(1) = 2; a(n) = smallest number such that the n-th partial
 *          concatenation is a multiple of prime(n).
 * https://oeis.org/A088632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Validated: standard, pass, 84 terms through n = 199 (2026-11-03)
 * Helpers:   ipow_u64_idx, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = smallest non‑negative integer x such that 
 *   concat( a(1),a(2),...,a(n) ) is a multiple of prime(n).
 *   a(1) = 2.
 * The function uses a cache to store computed a(i) values and 
 * recomputes the required concatenation modulo the current prime. 
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

static seq_t *cache = NULL;   /* stored a(i) values                    */
static size_t cache_len = 0;  /* number of initialized entries         */
static size_t cache_cap = 0;  /* allocated capacity                    */

/* ensure cache can hold at least 'upto' entries                */
static void ensure_cache(idx_t upto)
{
    if (upto <= cache_cap) return;
    size_t new_cap = (upto > 2) ? upto * 2 : 4;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;
    cache_cap = new_cap;
}

/* compute the nth term of the sequence                        */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    /* return cached value if already computed                 */
    if (n <= (idx_t)cache_len) return cache[n-1];

    /* compute terms up to n                                   */
    for (idx_t k = (idx_t)(cache_len + 1); k <= n; ++k)
    {
        if (k == (idx_t)1)            /* a(1) = 2                    */
        {
            ensure_cache(1);
            cache[0] = (seq_t)2;
            cache_len = 1;
            continue;
        }

        /* Current prime p = prime(k)                           */
        uint32_t p32 = get_prime(k);
        seq_t   mp   = (seq_t)p32;          /* cast to seq_t             */

        /* Compute C_{k-1} mod p by iterating over previous terms */
        seq_t c_mod = 0;
        for (idx_t i = 0; i < k-1; ++i)
        {
            seq_t ai = cache[i];
            int    dlen = num_digits(ai);

            /* pow10mod = 10^dlen mod p                          */
            seq_t pow10mod = 1;
            for (int j = 0; j < dlen; ++j)
                pow10mod = (pow10mod * 10ULL) % mp;

            c_mod = (c_mod * pow10mod + ai) % mp;
        }

        /* Find smallest x with n digits (leading zeros allowed)
           such that (C_{k-1} * 10^d + x) ≡ 0 (mod p)          */
        seq_t  result = 0;
        seq_t  pow10mod_d = 10ULL % mp;   /* 10^1 mod p           */
        idx_t  d = 1;

        for (;; ++d)
        {
            /* Compute r = (-C_{k-1} * 10^d) mod p                */
            seq_t tmp = (c_mod * pow10mod_d) % mp;
            seq_t r   = (tmp == 0) ? 0 : mp - tmp;

            /* Determine digit bounds for length d              */
            seq_t low  = (d == 1) ? (seq_t)0 : ipow_u64_idx(10ULL, d-1);
            seq_t high = ipow_u64_idx(10ULL, d) - 1;

            /* Smallest x ≥ low that is ≡ r (mod p)             */
            seq_t x;
            if (r < low)
            {
                seq_t diff   = low - r;
                seq_t steps  = (diff + (mp - 1)) / mp; /* ceil(diff/p) */
                x = r + steps * mp;
            }
            else
            {
                x = r;
            }

            if (x <= high)                /* valid choice found   */
            {
                result = x;
                break;
            }

            /* Next length: multiply previous 10^d by 10 mod p   */
            pow10mod_d = (pow10mod_d * 10ULL) % mp;
        }

        /* Store the computed term */
        ensure_cache(k);
        cache[k-1] = result;
        if (cache_len < (size_t)k) cache_len = (size_t)k;
    }

    return cache[n-1];
}
