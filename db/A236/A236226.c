/*
 * A236226  Sum of the eighteenth powers of the first n primes.
 * https://oeis.org/A236226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..n} prime(k)^18
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 5, u128 n <= 32,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 984 (2026-11-01)
 * Helpers:   ipow_u64_idx, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1, but guard against n == 0 anyway. */
    if (n == 0)
        return (seq_t)0;

    /* Obtain the first n primes (stored as 32‑bit integers). */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);

    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        /* Compute prime^18 using the helper; cast to 64‑bit for the base. */
        sum += ipow_u64_idx((uint64_t)primes[i], 18);
    }

    free(primes);
    return sum;
}
