/*
 * A051077  Primes p such that x^16 = -2 has a solution mod p.
 * https://oeis.org/A051077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A051077: primes p such that x^16 = -2 has a solution mod p
   The sequence is defined by this property; there is no closed‑form
   formula.  The implementation builds the list of such primes on
   demand, caching results so that any call order remains correct. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"
#include "../utils/grow_seq.h"

/* Cached list of terms.  All values are stored in seq_t, which may
   be a primitive unsigned integer or an arbitrary‑precision class. */
static seq_t *terms  = NULL;   /* array of computed terms                     */
static size_t terms_size = 0;  /* number of terms stored                     */
static size_t terms_cap  = 0;  /* current capacity of 'terms'                */
static uint64_t nxt = 3;       /* next odd candidate to test (after 2)       */
static bool initialized = false; /* whether the cache has been primed       */

/* Ensure that the array has capacity for at least min_cap elements.
   Uses grow_seq, which works with any seq_t type. */
static void ensure_terms_capacity(size_t min_cap)
{
    if (terms_cap < min_cap)
        grow_seq(&terms, &terms_cap, min_cap);
}

/* For a given prime p, return true iff there exists an integer x with
   x^16 ≡ -2 (mod p).  For p=2 the congruence is trivially satisfied. */
static bool check_condition(uint64_t p)
{
    if (p == 2ULL) return true;

    /* Compute d = gcd(16, p-1).  Since 16 = 2^4, this is 2^min(v2(p-1),4). */
    unsigned v = __builtin_ctzll(p - 1);
    uint64_t d = 1ULL << (v < 4 ? v : 4);

    uint64_t exp = (p - 1) / d;           /* exponent to test               */
    seq_t base   = (seq_t)(p - 2);        /* -2 modulo p                    */
    seq_t mod    = (seq_t)p;
    seq_t power  = mod_pow(base, exp, mod);

    return power == (seq_t)1;
}

/* Generate and cache terms until at least n such primes are known. */
static void compute_terms_up_to(idx_t n)
{
    if (!initialized)
    {
        /* Prime 2 is the first term. */
        ensure_terms_capacity(1);
        terms[0] = (seq_t)2;
        terms_size = 1;
        initialized = true;
        nxt = 3;   /* next odd candidate to test */
    }

    while (terms_size < (size_t)n)
    {
        /* Search for the next prime satisfying the congruence. */
        while (true)
        {
            if (is_prime((idx_t)nxt))
            {
                if (check_condition(nxt))
                {
                    ensure_terms_capacity(terms_size + 1);
                    terms[terms_size++] = (seq_t)nxt;
                    nxt += 2;   /* skip even numbers afterwards */
                    break;
                }
            }
            nxt += 2;   /* continue searching odd numbers */
        }
    }
}

/* Public entry point.  n>=1 as per OEIS offset. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* sentinel for out‑of‑bounds (offset 1) */
    compute_terms_up_to(n);
    return terms[n - 1];
}
