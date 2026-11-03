/*
 * A095181  Triangle read by rows in which the j-th row begins with prime(j)
 *          and is the arithmetic progression with least common difference
 *          such that the remaining j-1 terms are composite and not divisible
 *          by prime(j).
 * https://oeis.org/A095181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Build the triangle row by row. For the j‑th row the first
 *            element is the j‑th prime. Find the smallest positive difference
 *            d such that the arithmetic progression p, p+d, p+2d, … ,
 *            p+(j-1)d has the first term prime and all subsequent terms
 *            composite and not divisible by p. All rows are concatenate...
 * Range:     u8 n <= 22, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes required for static allocation and booleans */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Utility functions supplied by the build environment */

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

/* Function to compute the n-th term of A095181 */
seq_t a(idx_t n)
{
    /* Static cache for all computed terms */
    static seq_t *cache   = NULL;   /* array of terms   */
    static size_t cache_size = 0;   /* number of terms stored  */
    static size_t cache_cap  = 0;   /* allocated capacity      */
    static idx_t   next_row = 1;    /* next j to generate      */

    /* Generate rows until we have at least n terms */
    while (cache_size < n)
    {
        idx_t j = next_row;                     /* row length              */
        seq_t p = (seq_t)get_prime(j);          /* first term of row j     */
        seq_t d = 1;                            /* trial common difference */

        /* Search for the minimal d  */
        for (;; ++d)
        {
            bool ok = true;
            /* test all remaining j-1 terms */
            for (idx_t m = 1; m < j; ++m)
            {
                seq_t term = p + (seq_t)m * d;   /* arithmetic progression */
                if (term % p == 0) {            /* divisible by the prime   */
                    ok = false;
                    break;
                }
                if (is_prime_big(term)) {        /* term is still prime     */
                    ok = false;
                    break;
                }
            }
            if (ok)
                break;                         /* suitable d found        */
        }

        /* Append this row's terms to the cache */
        for (idx_t k = 0; k < j; ++k)
        {
            seq_t val = p + (seq_t)k * d;

            /* Ensure enough storage for the new term */
            if (cache_size >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)                      /* allocation failed  */
                    return 0;                  /* abort gracefully   */
                cache    = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = val;         /* store the term      */
        }
        ++next_row;                            /* move to next row    */
    }

    return cache[n - 1];    /* zero‑based indexing into the cache */
}
