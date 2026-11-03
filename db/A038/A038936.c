/*
 * A038936  Primes p such that 57 is not a square mod p.
 * https://oeis.org/A038936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 990 (2026-11-01)
 * Helpers:   is_prime_seq, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th prime p such that 57 is a quadratic
 * non‑residue modulo p.  The Legendre symbol (57|p) = –1 is tested
 * via Euler's criterion: 57^((p‑1)/2) mod p = p‑1.
 * The sequence is generated on demand and cached for repeated calls.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"
#include "../utils/powmod_u64.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache   = NULL;   /* array of found primes */
static size_t  cap    = 0;      /* current array capacity      */
static size_t  count  = 0;      /* number of primes stored     */
static uint64_t next_candidate = 3;   /* next odd integer to test */

/* Ensure that 'count' contains at least 'n' qualifying primes */
static void ensure_terms(idx_t n)
{
    while (count < n)
    {
        uint64_t p = next_candidate;

        /* Find the next prime */
        while (!is_prime_seq((seq_t)p))
            p += 2;

        /* Test if 57 is a quadratic non‑residue mod p */
        uint64_t leg = powmod_u64(57ULL, (p - 1) / 2, p);
        if (leg == p - 1)                  /* Legendre symbol = -1 */
        {
            if (count == cap)              /* grow cache */
            {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                if (!tmp)
                    abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[count++] = (seq_t)p;
        }

        /* Advance to the next odd integer after the prime examined */
        next_candidate = p + 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined for n = 0 */
    ensure_terms(n);
    return cache[n - 1];
}
