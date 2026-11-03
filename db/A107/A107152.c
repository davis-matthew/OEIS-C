/*
 * A107152  Primes of the form x^2 + 45y^2.
 * https://oeis.org/A107152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime of the form x^2 + 45y^2, i.e. primes that
 *            are ≡ 1 or 49 (mod 60)
 * Range:     u8 n <= 5, u16 n <= 789, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9970 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* Dynamic array helpers. */
static void push_seq(seq_t **arr, idx_t *cap, idx_t *size, seq_t val)
{
    if (*size >= *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
    (*arr)[*size] = val;
    (*size)++;
}

/* static storage for the primes found so far */
static seq_t *prime_vals = NULL;   /* dynamic array of primes */
static idx_t prime_size = 0;       /* number of primes stored */
static idx_t prime_cap  = 0;       /* capacity of the array */

/* candidate from which we start the search for the next prime */
static uint64_t next_candidate = 61;

/* Ensure that at least 'n' primes have been computed and stored. */
static void ensure_primes(idx_t n)
{
    while (prime_size < n)
    {
        /* Find the next prime of the required form. */
        while (true)
        {
            /* If the candidate is suitable, test for primality. */
            if ((next_candidate % 60 == 1) || (next_candidate % 60 == 49))
            {
                if (is_prime_u64(next_candidate))
                {
                    /* Store it. */
                    push_seq(&prime_vals, &prime_cap, &prime_size,
                             (seq_t)next_candidate);
                    /* Prepare the next candidate. */
                    next_candidate += 2;
                    break;
                }
            }
            /* step to the next odd number. */
            next_candidate += 2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                /* offset starts at 1 – undefined for 0 */
        return 0;

    ensure_primes(n);
    return prime_vals[n - 1];
}
