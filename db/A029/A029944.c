/*
 * A029944  a(n)^2 contains n-th prime as a substring.
 * https://oeis.org/A029944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 8152, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9902 (2026-11-03)
 * Helpers:   get_digits, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation by brute force search for smallest a such that a^2
   contains the nth prime as a decimal substring.
   a(1) is the first term, etc. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/get_digits.h"
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

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* allocated size (1‑based) */

    /* allocate or grow cache if needed */
    if ((size_t)n >= cache_size)
    {
        size_t new_size = (size_t)n + 1;   /* inclusive for index n */
        seq_t *new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache)
            return 0;                       /* allocation failed */

        /* initialise new entries to 0 (not computed) */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;

        cache = new_cache;
        cache_size = new_size;
    }

    /* return cached value if already computed */
    if (cache[n])
        return cache[n];

    /* get the nth prime as seq_t */
    uint32_t prime_u32 = get_prime(n);
    seq_t    prime     = (seq_t)prime_u32;

    /* extract decimal digits of the prime */
    uint8_t  prime_buf[64];
    size_t   prime_len = 0;
    get_digits(prime, prime_buf, &prime_len);

    /* search for smallest a */
    seq_t a_val = 1;
    while (1)
    {
        /* square */
        seq_t sq = seq_mul(a_val, a_val);

        /* digits of square */
        uint8_t  sq_buf[256];
        size_t   sq_len = 0;
        get_digits(sq, sq_buf, &sq_len);

        if (sq_len >= prime_len)
        {
            /* check for prime substring */
            bool has = false;
            for (size_t i = 0; i <= sq_len - prime_len; ++i)
            {
                size_t j;
                for (j = 0; j < prime_len; ++j)
                {
                    if (sq_buf[i + j] != prime_buf[j])
                        break;
                }
                if (j == prime_len)
                {
                    has = true;
                    break;
                }
            }
            if (has)
            {
                cache[n] = a_val;
                return a_val;
            }
        }
        ++a_val;
    }
}
