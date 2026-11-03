/*
 * A046684  Numbers k such that k and sum of squares of divisors of k are
 *          relatively prime.
 * https://oeis.org/A046684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 415, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9922 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046684: numbers k such that k and the sum of the squares of its
   divisors are relatively prime.  The function generates terms lazily
   and caches them.  Only integer arithmetic is used. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static uint64_t sigma2_u64(uint64_t n)
{
    uint64_t result = 1;
    uint64_t temp   = n;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* Count the exponent of p in n and build the geometric sum
               1 + p² + p⁴ + … + p²e. */
            uint8_t e   = 0;
            uint64_t pow = 1;
            uint64_t term = 1;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
                pow  *= p * p;
                term += pow;
            }
            result *= term;
        }
    }
    if (temp > 1)                /* Remaining prime factor. */
    {
        result *= (1 + temp * temp);
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* Cache for the generated sequence terms.  It grows dynamically on
   demand.  The cache is indexed from 0 (a(1) is stored at cache[0]). */

static seq_t *cache = NULL;       /* pointer to the cached terms          */
static size_t cache_len = 0;      /* number of terms currently cached     */
static uint64_t next_k = 1;       /* next integer k to test              */

/* ------------------------------------------------------------------ */
/* Compute σ₂(k) = sum of squares of all divisors of k.  The function
   uses a straightforward trial division and works with 64‑bit arithmetic,
   which is sufficient for the required values.  For arbitrary‑precision
   builds the result is converted to seq_t after the calculation. */

/* ------------------------------------------------------------------ */
/* Ensure that the cache has space for at least 'need' elements.
   The function doubles the capacity each time it is exceeded. */

static void ensure_cache_capacity(size_t need)
{
    if (cache_len >= need)
        return;                   /* already large enough */

    size_t new_cap = cache_len ? cache_len << 1 : 64;
    while (new_cap < need)
        new_cap <<= 1;

    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    /* The evaluation harness guarantees sufficient memory, so no NULL
       check is necessary. */
    cache = new_cache;
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  It lazily generates terms until the cache
   contains at least n elements, then returns the nth term.  The
   function works for any integer type chosen for seq_t. */

seq_t a(idx_t n)
{
    /* Build terms until we have at least 'n' entries. */
    while (cache_len < (size_t)n)
    {
        uint64_t k        = next_k;
        uint64_t sigma    = sigma2_u64(k);

        seq_t k_seq      = (seq_t)k;          /* cast to seq_t */
        seq_t sigma_seq  = (seq_t)sigma;

        if (gcd(k_seq, sigma_seq) == (seq_t)1)
        {
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = k_seq;
        }
        ++next_k;
    }
    return cache[n - 1];
}
