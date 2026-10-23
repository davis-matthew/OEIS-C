/*
 * A123082  Prime trios of form k, k+6, k+12.
 * https://oeis.org/A123082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A123082 – Prime trios of the form k, k+6, k+12.
 * Returns the n‑th prime that appears in the ordered list of
 * all such trios: each trio contributes its k, k+6, k+12 in that
 * order.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static bool *prime_flags = NULL;   /* sieve table */
static size_t max_limit = 0;       /* current sieve size */

/* Build a sieve of Eratosthenes up to 'lim'.  Overwrites prime_flags. */
static void build_sieve(size_t lim)
{
    if (prime_flags)
        free(prime_flags);

    prime_flags = (bool *)malloc((lim + 1) * sizeof(bool));
    if (!prime_flags)
        exit(2);                       /* memory error – abort */

    for (size_t i = 0; i <= lim; ++i)
        prime_flags[i] = true;

    if (lim >= 0) prime_flags[0] = false;
    if (lim >= 1) prime_flags[1] = false;

    for (size_t p = 2; p * p <= lim; ++p) {
        if (prime_flags[p]) {
            for (size_t m = p * p; m <= lim; m += p)
                prime_flags[m] = false;
        }
    }
    max_limit = lim;
}

/* Return the n-th term of A123082. */
seq_t a(idx_t n)
{
    /* Ensure we have at least a modest initial sieve. */
    if (!prime_flags)
        build_sieve(200000);

    const size_t target = (size_t)n;
    while (1) {
        size_t count = 0;
        for (size_t k = 5; k + 12 <= max_limit; ++k) {
            if (prime_flags[k] && prime_flags[k + 6] && prime_flags[k + 12]) {
                if (++count == target) return (seq_t)k;
                if (++count == target) return (seq_t)(k + 6);
                if (++count == target) return (seq_t)(k + 12);
            }
        }
        /* Not enough terms yet – double the sieve size and try again. */
        size_t new_lim = (max_limit == 0) ? 200000 : max_limit * 2;
        build_sieve(new_lim);
    }
}
