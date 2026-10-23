/*
 * A035242  Indices of the nonzero terms in expansion of Dirichlet series
 *          Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m=
 *          -21.
 * https://oeis.org/A035242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A035242 – indices of the non‑zero terms in the expansion of
    ∏p (1-(χ(p)+1)p^(−s)+χ(p)p^(−2s))^(−1)  for m = −21.
    χ(p) is the Kronecker symbol (−21|p).
    A number n belongs to the sequence iff every prime p with
    χ(p)=−1 occurs in the factorisation of n with an even exponent. */

/*  We generate the numbers incrementally, testing each integer for
    the parity condition.  The sequence is cached so that arbitrary
    (possibly out‑of‑order) queries are answered quickly.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/jacobi_symbol.h"

/* ---- Prime infrastructure --------------------------------------- */
static uint32_t *spf = NULL;          /* smallest prime factor array   */
static idx_t spf_limit = 0;           /* length of spf                 */

static uint32_t *prime_list = NULL;   /* list of primes in the sieve   */
static size_t   prime_cap  = 0;       /* capacity of prime_list        */
static size_t   prime_cnt  = 0;       /* current count of primes       */

/* Grow the spf array and recompute the sieve up to new_limit.   */
static void build_spf(idx_t new_limit)
{
    if (new_limit <= spf_limit)
        return;                       /* nothing to do               */

    spf = (uint32_t *)realloc(spf, (new_limit + 1) * sizeof(uint32_t));
    /* initialise new part */
    for (idx_t i = spf_limit + 1; i <= new_limit; ++i)
        spf[i] = 0;

    /* linear sieve */
    for (idx_t i = 2; i <= new_limit; ++i)
    {
        if (spf[i] == 0) {             /* i is prime */
            spf[i] = (uint32_t)i;
            if (prime_cnt >= prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 64;
                prime_list = (uint32_t *)realloc(prime_list,
                                                  prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j)
        {
            idx_t prod = i * (idx_t)prime_list[j];
            if (prod > new_limit)
                break;
            spf[prod] = prime_list[j];
            if (prime_list[j] == spf[i])
                break;
        }
    }
    spf_limit = new_limit;
}

/* ---- Kronecker symbol for m = -21 -------------------------------- */
static int chi_from_prime(uint32_t p)
{
    if (p == 2)                  /* (−21|2) = −1   */
        return -1;
    if (p == 3 || p == 7)        /* factors of 21  */
        return 0;

    /* (−21|p) = (−1|p) · (21|p) */
    int s = (p % 4 == 1) ? 1 : -1;          /* (−1|p)   */
    int t = jacobi_symbol(21, p);           /* (21|p)   */
    return s * t;
}

/* ---- Test whether a number satisfies the parity condition ----- */
static bool is_valid(idx_t x)
{
    if (x == 1)
        return true;
    /* factorise using spf, growing the sieve as needed */
    build_spf(x);
    while (x > 1)
    {
        uint32_t p = spf[x];
        idx_t exp = 0;
        while (x % p == 0) {
            x /= p;
            ++exp;
        }
        if (chi_from_prime(p) == -1 && (exp & 1))
            return false;            /* odd exponent of a bad prime */
    }
    return true;
}

/* ---- Cache for computed terms ----------------------------------- */
static seq_t *cache   = NULL;   /* array of found terms          */
static size_t cache_len  = 0;   /* number of terms stored        */
static size_t cache_cap  = 0;   /* capacity of cache array       */
static idx_t  next_scan  = 1;   /* next integer to test          */

/* Grow cache array */
static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ---- Main routine ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for index 0 */
        return 0;

    /* return cached value if available */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* extend cache until we have computed the n‑th term */
    while (cache_len < (size_t)n) {
        if (is_valid(next_scan)) {
            if (cache_len == cache_cap)
                cache_grow();
            cache[cache_len++] = (seq_t)next_scan;
        }
        ++next_scan;
    }
    return cache[n - 1];
}
