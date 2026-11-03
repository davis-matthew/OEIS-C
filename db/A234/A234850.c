/*
 * A234850  Primes in A014692, i.e., of the form prime(k)-k+1, for some k.
 * https://oeis.org/A234850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form prime(k) - k + 1 (the primes
 *            occurring in the sequence A014692)
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
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

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

seq_t a(idx_t n)
{
    static idx_t cache_size = 0;           /* number of stored terms     */
    static idx_t cache_cap   = 0;           /* allocated size            */
    static seq_t *cache      = NULL;        /* cache array               */
    static idx_t last_k      = 1;           /* next k to test            */

    /* accumulate terms until we have at least n of them */
    while (cache_size < n) {
        /* ensure sufficient capacity */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = mem_new(seq_t, cache_cap);
        } else if (cache_size == cache_cap) {
            cache_cap <<= 1;
            cache = mem_resize(seq_t, cache, cache_cap);
        }

        /* compute candidate value prime(k) - k + 1 */
        seq_t pk  = (seq_t)get_prime(last_k);   /* prime(k)          */
        seq_t val = pk - (seq_t)last_k + 1;     /* candidate value   */

        /* keep it if it is prime */
        if (isPrimeGeneric(val))
            cache[cache_size++] = val;

        ++last_k;   /* next k to test */
    }

    return cache[n - 1];   /* a(1) is at index 0 */
}
