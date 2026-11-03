/*
 * A064250  Values of m such that N=(am+1)(bm+1)(cm+1) is a 3-Carmichael
 *          number (A087788), where a,b,c = 1,2,27.
 * https://oeis.org/A064250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 50 terms through n = 8151 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A064250:  m such that N=(m+1)(2m+1)(27m+1) is a 3‑Carmichael number.
   Conditions reduce to:
       1. m ≡ 12 (mod 54)          // m even and 2m+1|N-1 and 27m+1|N-1
       2. m+1, 2m+1, 27m+1 are primes
   The function generates these m in order, caching results for re‑use.
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static idx_t  cache_cap  = 0;
    /* current candidate starting at 12 (first member of the arithmetic progression) */
    static seq_t cur_m = 12;

    /* generate terms until we have at least n of them */
    while (cache_size < n)
    {
        /* ensure cache capacity */
        if (cache_size == cache_cap)
        {
            idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) abort();            /* unrecoverable allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* check current candidate cur_m */
        seq_t m = cur_m;
        cur_m += 54;                 /* next term in the progression */

        /* compute the three linear forms */
        seq_t p1 = m + 1;            /* m+1    */
        seq_t p2 = (m * 2) + 1;      /* 2m+1   */
        seq_t p3 = (m * 27) + 1;     /* 27m+1  */

        /* primality tests */
        if (  isPrimeGeneric(p1)
           && isPrimeGeneric(p2)
           && isPrimeGeneric(p3) )
        {
            /* store m as a term of the sequence */
            cache[cache_size++] = m;
        }
    }

    return cache[n-1];
}
