/*
 * A142507  Primes congruent to 50 mod 51.
 * https://oeis.org/A142507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime p such that p mod 51 == 50
 * Range:     u8 n <= 1, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Sequence entry function: returns the n-th prime congruent to 50 modulo 51 */
/* idx_t is the index type (uint64_t), seq_t is the value type (unsigned integer
   or big‑integer class).  The function may be called with n >= 1 in any order. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;                       /* offset starts at 1; n=0 is undefined */
    }

    /* Static storage for previously computed primes in the progression. */
    static seq_t *values   = NULL;        /* array of primes found so far        */
    static size_t count    = 0;           /* number of primes stored             */
    static size_t capacity = 0;           /* allocated length of values array    */

    /* Ensure we have at least n primes stored.  If not, extend the sieve. */
    if (count < (size_t)n) {
        /* Rough estimate from the prime number theorem for arithmetic
           progressions: the n-th prime ≈ 32 * n * log n.  We use a generous
           multiplier to be safe. */
        size_t est = n * 600 + 1000;
        if (est < 1000) est = 1000;

        while (1) {
            /* Allocate a Boolean array for the sieve of size est + 1. */
            bool *comp = (bool *)calloc(est + 1, sizeof(bool));
            if (!comp) {
                /* Allocation failure – return 0 as a sentinel. */
                return 0;
            }

            /* Simple Eratosthenes sieve. */
            for (size_t i = 2; i <= est / i; ++i) {
                if (!comp[i]) {
                    for (size_t j = i * i; j <= est; j += i) {
                        comp[j] = true;
                    }
                }
            }

            /* Count how many primes of the desired form appear. */
            size_t local_count = 0;
            for (size_t i = 2; i <= est; ++i) {
                if (!comp[i] && (i % 51) == 50) {
                    ++local_count;
                }
            }

            if (local_count >= (size_t)n) {
                /* Resize the values array if needed. */
                if (capacity < local_count) {
                    seq_t *new_vals = (seq_t *)calloc(local_count, sizeof(seq_t));
                    if (!new_vals) {
                        free(comp);
                        return 0;            /* allocation failure */
                    }
                    free(values);
                    values   = new_vals;
                    capacity = local_count;
                }

                /* Store the qualifying primes in order. */
                size_t idx = 0;
                for (size_t i = 2; i <= est; ++i) {
                    if (!comp[i] && (i % 51) == 50) {
                        values[idx++] = (seq_t)i;
                    }
                }
                count = local_count;
                free(comp);
                break;   /* we now have enough primes */
            }

            /* Not enough primes – increase the bound and try again. */
            free(comp);
            est *= 2;
        }
    }

    /* Return the requested n-th term (indices are 1‑based). */
    return values[n - 1];
}
