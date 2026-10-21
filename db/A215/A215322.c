/*
 * A215322  Primes congruent to {1, 2, 3, 4, 6} mod 11.
 * https://oeis.org/A215322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A215322: primes congruent to {1,2,3,4,6} mod 11 */
/* This function generates the terms by iterating over integers,
   testing primality with a trial‑division list that is grown lazily,
   and selecting those primes whose remainder mod 11 is 1,2,3,4 or 6. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* --- Helper: dynamic array growth ------------------------------------- */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *p = realloc(*ptr, new_cap * elem_size);
    if (!p) exit(1);              /* insufficient memory */
    *ptr = p;
    *cap = new_cap;
}

/* --- State: primes list and sequence list ---------------------------- */
static uint64_t       *primes         = NULL; /* all primes found so far (uint64_t) */
static size_t         prime_len      = 0;    /* current number of primes          */
static size_t         prime_cap      = 0;    /* current capacity of primes array  */

static seq_t         *seq_primes      = NULL; /* sequence primes (residue condition) */
static size_t         seq_len        = 0;    /* number of sequence terms stored   */
static size_t         seq_cap        = 0;    /* capacity of seq_primes            */

/* --- Current candidate ----------------------------------------------- */
static uint64_t       next_candidate  = 2;    /* next integer to test for primality */

/* --- Test primality using the growing list of primes ----------------- */
static bool is_prime_uint64(uint64_t p)
{
    if (p < 2)           return false;
    if (p == 2)          return true;
    if (p % 2 == 0)      return false;

    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t pr = primes[i];
        if (pr * pr > p) break;               /* overflow safe: p is small (≤ ~200000) */
        if (p % pr == 0) return false;
    }
    return true;
}

/* --- Function to return the nth term --------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure the sequence contains at least n terms */
    while (seq_len < n) {
        uint64_t p = next_candidate;

        if (is_prime_uint64(p)) {
            /* Append to the list of all primes */
            if (prime_len + 1 > prime_cap) {
                ensure_capacity(prime_len + 1, &prime_cap,
                                (void **)&primes, sizeof(uint64_t));
            }
            primes[prime_len++] = p;

            /* Check the residue condition and store in seq_primes if true */
            unsigned rem = (unsigned)(p % 11);
            if (rem == 1 || rem == 2 || rem == 3 || rem == 4 || rem == 6) {
                if (seq_len + 1 > seq_cap) {
                    ensure_capacity(seq_len + 1, &seq_cap,
                                    (void **)&seq_primes, sizeof(seq_t));
                }
                /* Cast from uint64_t to seq_t (implicit conversion handles big int) */
                seq_primes[seq_len++] = p;
            }
        }

        /* Advance to the next candidate:
           start from 2, then test only odd numbers */
        if (next_candidate == 2)
            next_candidate = 3;
        else
            next_candidate += 2;
    }

    return seq_primes[n - 1];
}
