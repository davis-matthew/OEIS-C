/*
 * A144524  Triangular numbers n*(n+1)/2 with n composite, where number of
 *          prime factors of n, counted with multiplicity, is less than the
 *          number of prime factors in n+1.
 * https://oeis.org/A144524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 where n is the nth composite integer with *
 *            Ω(n) < Ω(n+1) (Ω = number of prime factors counted with
 *            multiplicity)
 * Range:     u8 n <= 1, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   count_prime_factors, ensure_capacity, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility headers provided by the build system */
#include "../utils/is_prime.h"
#include "../utils/count_prime_factors.h"
#include "../utils/ensure_capacity.h"

/* Cache of computed terms.  tri_cache[i] stores a(i+1). */
static seq_t *tri_cache = NULL;   /* dynamic array of sequence values */
static size_t tri_len   = 0;       /* number of terms stored */
static size_t tri_cap   = 0;       /* current capacity of tri_cache */

/* Next integer to test (starts just after the first composite number). */
static idx_t next_to_test = 4ULL;

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    /* Grow the cache until it contains at least n terms. */
    while (tri_len < n) {
        /* Scan successive integers until one satisfies the condition. */
        for (; next_to_test <= UINT64_MAX - 1; ++next_to_test) {
            /* Skip primes. */
            if (!is_prime(next_to_test)) {
                seq_t cnt_n     = count_prime_factors((seq_t)next_to_test);
                seq_t cnt_nplus = count_prime_factors((seq_t)(next_to_test + 1));

                if (cnt_n < cnt_nplus) {
                    /* Compute the triangular number using seq_t arithmetic. */
                    seq_t tn = (seq_t)next_to_test * ((seq_t)next_to_test + 1) / 2;

                    /* Ensure there is room for the new term. */
                    ensure_capacity(tri_len + 1, &tri_cap,
                                    (void **)&tri_cache, sizeof(seq_t));

                    tri_cache[tri_len++] = tn;

                    /* Move to the next integer for subsequent searches. */
                    next_to_test++;
                    break;  /* found one; return to the while loop */
                }
            }
        }
    }

    /* Return the requested term (1‑based index). */
    return tri_cache[n - 1];
}
