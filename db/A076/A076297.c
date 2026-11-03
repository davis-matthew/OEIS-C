/*
 * A076297  Numbers k such that prime(k) + s*k is prime, s=2.
 * https://oeis.org/A076297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 2139, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9927 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th integer k such that prime(k) + 2*k is prime. */
/*  The offset is 1, a(1) is the first such integer. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
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

/* ----------------------------------------------------------------------
   Globals: cached primes and qualifying k values
   ---------------------------------------------------------------------- */
static uint32_t *primes = NULL;   /* primes[0 .. primes_len-1] are the first primes */
static size_t   primes_len = 0;
static size_t   primes_cap = 0;

static seq_t  *qual = NULL;      /* qualifying k values, 0‑based */
static size_t  qual_len = 0;
static size_t  qual_cap = 0;

static idx_t   curk = 0;        /* largest k that has been tested */

/* ----------------------------------------------------------------------
   Grow the primes array and fill it up to at least k primes
   ---------------------------------------------------------------------- */
static void
grow_primes_to(idx_t k)
{
    if (primes_len >= (size_t)k)
        return;

    /* Ensure capacity */
    if (primes_cap < (size_t)k) {
        size_t new_cap = primes_cap ? (primes_cap << 1) : 64;
        while (new_cap < (size_t)k)
            new_cap <<= 1;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Find new primes */
    uint32_t cand = (primes_len == 0) ? 2 : primes[primes_len - 1] + 1;
    while (primes_len < (size_t)k) {
        bool isp = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > cand)
                break;
            if (cand % p == 0) {
                isp = false;
                break;
            }
        }
        if (isp)
            primes[primes_len++] = cand;
        ++cand;
    }
}

/* ----------------------------------------------------------------------
   Ensure that the array `qual` contains at least n qualifying k values
   ---------------------------------------------------------------------- */
static void
ensure_qual_n(idx_t n)
{
    while (qual_len < (size_t)n) {
        ++curk;                            /* test next k */
        grow_primes_to(curk);              /* get prime(curk) */
        seq_t pk = (seq_t)primes[curk - 1];
        seq_t test = pk + (seq_t)2 * curk;  /* prime(k) + 2*k */

        if (isPrimeGeneric(test)) {
            /* Append curk to qual */
            if (qual_cap <= qual_len) {
                size_t new_cap = qual_cap ? (qual_cap << 1) : 16;
                seq_t *tmp = mem_resize(seq_t, qual, new_cap);
                qual = tmp;
                qual_cap = new_cap;
            }
            qual[qual_len++] = (seq_t)curk;
        }
    }
}

/* ----------------------------------------------------------------------
   External function required by the framework
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n < 1; return 0 */
    ensure_qual_n(n);
    return qual[n - 1];
}
