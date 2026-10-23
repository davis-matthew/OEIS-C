/*
 * A164566  Primes p such that 7*p-6 and 7*p+6 are also prime numbers.
 * https://oeis.org/A164566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 431, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164566: primes p such that 7p-6 and 7p+6 are also prime.
   The function computes terms by an incremental search over primes
   and tests the two offset numbers for primality. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* --------------------------------------------------------------------- */
/* Static storage for already produced terms.                            */
/* --------------------------------------------------------------------- */
static seq_t *result_arr = NULL;      /* array of found terms      */
static size_t result_len = 0;          /* number of terms stored   */
static size_t result_cap = 0;          /* allocated size of array  */

/* Next integer to test for primality.                                   */
static idx_t next_trial = 2;

/* --------------------------------------------------------------------- */
/* Ensure that there is room for at least 'space' more results.          */
/* --------------------------------------------------------------------- */
static void _ensure_capacity(size_t space)
{
    if (result_cap - result_len >= space)
        return;
    size_t new_cap = result_cap ? result_cap * 2 : 16;
    while (new_cap - result_len < space)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(result_arr, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* memory allocation failure */
    result_arr = tmp;
    result_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Check whether a prime candidate satisfies the required conditions.    */
/* --------------------------------------------------------------------- */
static bool _candidate_valid(idx_t p)
{
    /* p itself must be prime (confirms here to be safe). */
    if (!is_prime(p))
        return false;
    idx_t low  = 7 * p - 6;
    idx_t high = 7 * p + 6;
    return is_prime(low) && is_prime(high);
}

/* --------------------------------------------------------------------- */
/* Main entry: returns the n-th term (1‑based) of the sequence.          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* undefined for n < 1 – return 0 as a placeholder. */
    if (n < 1)
        return 0;

    /* If we already have the requested term, return it. */
    if (result_len >= (size_t)n)
        return result_arr[(size_t)(n - 1)];

    /* Extend the list until we have at least n terms. */
    while (result_len < (size_t)n) {
        /* Search for the next prime candidate. */
        idx_t p;
        for (p = next_trial; ; ++p) {
            if (is_prime(p))
                break;
        }
        next_trial = p + 1;   /* start next search after this prime */

        /* If the prime satisfies the twin‑distance condition, store it. */
        if (_candidate_valid(p)) {
            _ensure_capacity(1);
            result_arr[result_len++] = (seq_t)p;
        }
    }

    return result_arr[(size_t)(n - 1)];
}
