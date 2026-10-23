/*
 * A198066  Square array read by antidiagonals, n>=1, k>=1; T(n,k) is the
 *          number of primes which are prime to n and are not strong divisors
 *          of k.
 * https://oeis.org/A198066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A198066: a(n) is the entry T(i,j) of the square array
   read by antidiagonals, where
        T(i,j) = #{ primes p <= i : gcd(p,i)=1 and p ∤ j }   (i,j >= 1) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_table = NULL;   /* primes <= current limit */
static size_t      prime_cnt   = 0;     /* number of primes in the table   */
static idx_t       prime_limit = 0;     /* current upper bound of primes   */

/* Ensure that the prime_table contains all primes <= limit.
   Free the old table if a larger limit is needed. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit) return;
    uint64_t *new_table = NULL;
    size_t   new_cnt   = 0;
    build_primes_upto(limit, &new_table, &new_cnt);
    if (prime_table != NULL) free(prime_table);
    prime_table = new_table;
    prime_cnt   = new_cnt;
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* Find the diagonal index 't' such that t(t+1)/2 >= n. */
    idx_t t = 1;
    while (t * (t + 1) / 2 < n) ++t;

    /* Offset along this diagonal (1‑based). */
    idx_t offset = n - (t - 1) * t / 2;

    /* Convert to array coordinates: (row i, column j). */
    idx_t i = t - offset + 1;   /* row index (>=1)   */
    idx_t j = offset;           /* column index (>=1) */

    /* Make sure we have all primes up to 't' available. */
    ensure_primes_up_to(t);

    /* Count primes p <= i with the required properties. */
    seq_t count = 0;
    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint64_t p = prime_table[idx];
        if (p > i) break;                /* primes are sorted */
        if (i % p == 0) continue;        /* not coprime to i   */
        if (j % p == 0) continue;        /* p divides j (strong divisor) */
        ++count;
    }
    return count;
}
