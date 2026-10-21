/*
 * A215307  Primes congruent to {1, 2, 3, 5} mod 13.
 * https://oeis.org/A215307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of primes congruent to {1, 2, 3, 5} modulo 13.
   The implementation is a lazy prime generator that keeps two
   static arrays: one for all primes seen so far and one for the
   filtered primes that satisfy the congruence condition.
   When a(n) is called the function grows the arrays until
   the n‑th filtered prime has been found. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper to ensure a buffer has the requested capacity. */
static void *ensure_capacity(void *ptr, size_t *cap, size_t elem_size, size_t needed)
{
    if (*cap >= needed) return ptr;
    size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
    while (newcap < needed) newcap <<= 1;
    ptr = realloc(ptr, newcap * elem_size);
    *cap = newcap;
    return ptr;
}

/* Test if candidate (uint64_t) is prime using trial division by primes
   stored in prime_list (global). */
static bool is_prime_candidate(uint64_t candidate, const seq_t *prime_list, size_t prime_size)
{
    if (candidate < 2) return false;
    for (size_t i = 0; i < prime_size; ++i) {
        uint64_t p = (uint64_t)prime_list[i];
        uint64_t pp = p * p;
        if (pp > candidate) break;
        if (candidate % p == 0) return false;
    }
    return true;
}

static seq_t *prime_list = NULL;   /* All primes seen so far (seq_t type). */
static size_t prime_size = 0;
static size_t prime_cap = 0;

static seq_t *filtered = NULL;     /* Primes that satisfy the mod 13 condition. */
static size_t filtered_size = 0;
static size_t filtered_cap = 0;

static uint64_t next_candidate = 2;   /* Next integer to test for primality. */

/* Return the nth prime congruent to 1,2,3,5 modulo 13.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but harmless */

    while (filtered_size < n) {
        /* Generate the next candidate prime. */
        uint64_t cand = next_candidate;
        /* Advance candidate for next call. */
        if (cand <= 2) {
            next_candidate = 3;
        } else {
            next_candidate = cand + 2;   /* skip even numbers */
        }

        if (is_prime_candidate(cand, prime_list, prime_size)) {
            /* Store the new prime. */
            prime_list = (seq_t *)ensure_capacity(prime_list, &prime_cap, sizeof(seq_t), prime_size + 1);
            prime_list[prime_size++] = (seq_t)cand;

            /* Check the residue condition. */
            uint64_t mod = cand % 13;
            if (mod == 1 || mod == 2 || mod == 3 || mod == 5) {
                filtered = (seq_t *)ensure_capacity(filtered, &filtered_cap, sizeof(seq_t), filtered_size + 1);
                filtered[filtered_size++] = (seq_t)cand;
            }
        }
    }

    return filtered[n - 1];
}
