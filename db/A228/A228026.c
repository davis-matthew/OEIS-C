/*
 * A228026  Primes of the form 4^k + 3.
 * https://oeis.org/A228026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Prime of the form 4^k + 3: a(n) = 4^k + 3 where 4^k+3 is the nth prime with that property.
 * (OEIS A228026)
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* ------------------------------------------------------------------ */
/* Static cache of found primes                                        */
/* ------------------------------------------------------------------ */
static seq_t *primes = NULL;      /* array of primes */
static size_t primes_cap = 0;     /* allocated capacity */
static size_t primes_len = 0;     /* number of primes stored */

/* Current power of four (4^k) used in the next candidate.  Starts at 4¹ = 4. */
static seq_t power_of_four = (seq_t)4;

/* ------------------------------------------------------------------ */
/* Ensure the cache has capacity for at least 'needed' entries.       */
/* ------------------------------------------------------------------ */
static void _ensure_cache_capacity(size_t needed)
{
    if (needed <= primes_cap)
        return;

    size_t new_cap = primes_cap ? primes_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_ptr = seq_realloc(primes, primes_cap, new_cap);
    if (!new_ptr)
        abort();                 /* out of memory */

    primes = new_ptr;
    primes_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Initialise static state only once.                                  */
/* ------------------------------------------------------------------ */
static void _init_static(void)
{
    static bool initialized = false;
    if (initialized)
        return;

    primes = seq_alloc_new_array(16);
    if (!primes)
        abort();                 /* out of memory */
    primes_cap = 16;
    primes_len = 0;
    power_of_four = (seq_t)4;
    initialized = true;
}

/* ------------------------------------------------------------------ */
/* Main implementation of a(idx_t n)                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* Undefined for offset < 1 */

    _init_static();

    /* Generate primes of form 4^k + 3 until we have at least 'n' of them. */
    while (primes_len < (size_t)n) {
        /* Candidate = 4^k + 3 */
        seq_t candidate = power_of_four + (seq_t)3;

        if (is_prime_big(candidate)) {
            _ensure_cache_capacity(primes_len + 1);
            primes[primes_len++] = candidate;
        }

        /* Prepare for the next exponent: multiply by 4 */
        power_of_four = power_of_four * (seq_t)4;
    }

    return primes[(size_t)n - 1];
}
