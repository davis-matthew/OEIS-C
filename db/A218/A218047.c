/*
 * A218047  Numbers n such that n^2+1, (n+2)^2+1, (n+6)^2+1, (n+10)^2+1 and
 *          (n+12)^2+1 are prime.
 * https://oeis.org/A218047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big, sq_plus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A218047: numbers n such that n²+1, (n+2)²+1, (n+6)²+1,
   (n+10)²+1 and (n+12)²+1 are all prime.
   Implementation: brute‑force search over n ≡ 4 (mod 10) with a fast
   primality test for 64‑bit values (deterministic Miller–Rabin) and a
   fallback to the library routine is_prime_big for arbitrary precision
   values.  The function is memoised so it works for repeated calls in
   any order. */

#include <stdlib.h>   /* realloc, free */
#include <stdbool.h>
#include <stdint.h>   /* uint64_t */
#include <stddef.h>   /* size_t */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/sq_plus_one.h"

static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t powmod64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1) r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        d >>= 1;
    }
    return r;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static inline bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t tiny[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof tiny / sizeof *tiny; ++i)
    {
        if (n % tiny[i] == 0) return n == tiny[i];
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* bases that make the test deterministic for 64‑bit ints */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = mulmod64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Fast primality test that handles both 64‑bit and arbitrary precision
   values by delegating to the appropriate routine. */
static inline bool is_prime_fast(seq_t v)
{
    if (v <= (seq_t)UINT64_MAX)
        return is_prime_64((uint64_t)v);
    return is_prime_big(v);
}

/* ------------------------------------------------------ */
/* Sequence routine */

seq_t a(idx_t n)
{
    /* ---------- static state ----------
       terms:   array of found terms
       cnt:     number of terms found
       next:    next n to test (≡ 4 (mod 10))
       ------------------------------------- */
    static seq_t *terms      = NULL;
    static size_t terms_cnt  = 0;
    static size_t terms_cap  = 0;
    static seq_t next_candidate = 4;
    static bool initialized = false;

    if (!initialized)
    {
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (terms_cnt < n)
    {
        /* check the five candidate values */
        static const idx_t offs[5] = {0, 2, 6, 10, 12};
        bool good = true;
        for (idx_t i = 0; i < 5; ++i)
        {
            seq_t t = next_candidate + offs[i];
            if (!is_prime_fast(sq_plus_one(t)))
            {
                good = false;
                break;
            }
        }

        if (good)
        {
            /* store the term */
            if (terms_cnt == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp)
                {   /* allocation failed – clean up and return 0 */
                    free(terms);
                    terms = NULL;
                    terms_cap = 0;
                    terms_cnt = 0;
                    return 0;
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_cnt++] = next_candidate;
        }

        /* advance to next candidate (always +10) */
        next_candidate += 10;
    }

    /* return the required term (1‑based index) */
    return terms[n - 1];
}
