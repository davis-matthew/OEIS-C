/*
 * A097282  Numbers k that are the hypotenuse of exactly 40 distinct
 *          integer-sided right triangles, i.e., k^2 can be written as a sum
 *          of two squares in 40 ways.
 * https://oeis.org/A097282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A097282 – numbers k that are the hypotenuse of exactly 40 distinct
   integer‑sided right triangles.  
   k^2 can be written as a sum of two squares in 40 distinct ways.
   This is equivalent to
      ∏(2e_i+1) = 81
   over the primes ≡1 (mod 4) in the factorisation of k. */

/* Standard headers: */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Small prime sieve – we only need primes up to 200 000.            */
/* ------------------------------------------------------------------ */
#define PRIMES_LIMIT 200000
static uint32_t primes[PRIMES_LIMIT];
static size_t primes_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;

    bool *is_composite = (bool*)calloc(PRIMES_LIMIT + 1, sizeof(bool));
    if (!is_composite) exit(1);

    for (uint32_t i = 2; i <= PRIMES_LIMIT; ++i)
    {
        if (!is_composite[i])
        {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= PRIMES_LIMIT)
            {
                for (uint32_t j = i * i; j <= PRIMES_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_built = true;
}

/* ------------------------------------------------------------------ */
/* Test whether a uint64_t value k satisfies the product = 81 property.*/
/* ------------------------------------------------------------------ */
static bool good_k_uint64(uint64_t k)
{
    uint64_t prod = 1;
    uint64_t x = k;

    for (size_t i = 0; i < primes_cnt; ++i)
    {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > x) break;

        if (x % p == 0)
        {
            uint32_t cnt = 0;
            while (x % p == 0)
            {
                x /= p;
                ++cnt;
            }
            if (p % 4 == 1)
            {
                prod *= (2U * cnt + 1U);
                if (prod > 81U) return false;
            }
        }
    }
    /* Remaining factor (if any) */
    if (x > 1)
    {
        if (x % 4 == 1)
        {
            prod *= 3U;
            if (prod > 81U) return false;
        }
        /* primes ≡3 (mod 4) or 2 do not affect prod */
    }
    return prod == 81U;
}

/* ------------------------------------------------------------------ */
/* Dynamic array of terms.                                            */
/* ------------------------------------------------------------------ */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t next_k = 2U;   /* next integer to test for good_k */

/* Ensure that at least n terms are generated. */
static void ensure_terms(idx_t n)
{
    if (n == 0 || (size_t)n <= terms_len) return;

    build_primes();

    /* Grow the terms array if needed. */
    if (terms == NULL)
    {
        terms_cap = 1024;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
        if (!terms) exit(1);
    }

    while (terms_len < (size_t)n)
    {
        /* Find the next k that satisfies the property. */
        while (!good_k_uint64(next_k))
            ++next_k;

        /* Insert into the array. */
        if (terms_len >= terms_cap)
        {
            terms_cap *= 2;
            terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            if (!terms) exit(1);
        }
        terms[terms_len++] = (seq_t)next_k;

        /* Advance to the next integer for the next search. */
        ++next_k;
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the exercise.                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety for offset‑1 */
    ensure_terms(n);
    return terms[(size_t)n - 1];
}
