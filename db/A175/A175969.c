/*
 * A175969  Complement of A014284(n), where A014284(n) = the lexicographically
 *          earliest sequence with first differences as increasing sequence of
 *          primes A000040.
 * https://oeis.org/A175969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A175969: Complement of A014284, the lexicographically earliest
 * sequence with first differences equal to the increasing primes.
 * The terms are all positive integers that are NOT equal to
 * 1 + the cumulative sums of the sequence of primes. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */

seq_t a(idx_t n)
{
    /* Undefined for n == 0 – return 0 by convention. */
    if (n == 0) return 0;

    /* Static state for the complement sequence. */
    static seq_t *comp = NULL;      /* array holding the complement terms   */
    static size_t comp_len = 0;     /* number of terms stored              */
    static size_t comp_cap = 0;     /* current allocated capacity          */

    /* Static state for the A014284 values. */
    static idx_t *primes = NULL;     /* list of primes used for differences */
    static size_t primes_len = 0;    /* number of primes stored             */
    static size_t primes_cap = 0;    /* allocated capacity for primes       */
    static seq_t current_s = 1;      /* current term of A014284             */
    static size_t prime_idx = 0;     /* index in primes array for next diff */
    static idx_t curr_number = 1;    /* next natural number to test         */

    /* Initialise static data on first use. */
    if (!comp) {
        comp = NULL;
        comp_cap = 0;
        comp_len = 0;

        primes = NULL;
        primes_cap = 0;
        primes_len = 0;

        /* Seed the prime list with 2. */
        size_t need = 4;
        primes = (idx_t*)realloc(primes, need * sizeof(idx_t));
        primes_cap = need;
        primes[primes_len++] = 2;

        current_s = 1;      /* first value of A014284 */
        prime_idx = 0;      /* next difference uses primes[0] = 2 */
        curr_number = 1;    /* start scanning natural numbers at 1 */
    }

    /* Extend the complement until we have at least n terms. */
    while (comp_len < (size_t)n) {
        if (curr_number == current_s) {
            /* Number belongs to A014284 – skip it and advance. */
            if (prime_idx >= primes_len) {
                /* Generate the next prime and append it. */
                idx_t cand = primes[primes_len - 1] + 1;
                while (!is_prime(cand)) ++cand;

                if (primes_cap <= primes_len) {
                    size_t new_cap = primes_cap ? primes_cap * 2 : 4;
                    primes = (idx_t*)realloc(primes, new_cap * sizeof(idx_t));
                    primes_cap = new_cap;
                }
                primes[primes_len++] = cand;
            }
            /* Advance the A014284 term by the next prime difference. */
            current_s += primes[prime_idx];
            ++prime_idx;
        } else {
            /* Number does NOT belong to A014284 – store it. */
            if (comp_cap <= comp_len) {
                size_t new_cap = comp_cap ? comp_cap * 2 : 64;
                comp = (seq_t*)realloc(comp, new_cap * sizeof(seq_t));
                comp_cap = new_cap;
            }
            comp[comp_len++] = curr_number;
        }
        ++curr_number;
    }

    return comp[n - 1];
}
