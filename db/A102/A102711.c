/*
 * A102711  Numbers k such that 11*k + 7 is prime.
 * https://oeis.org/A102711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A102711: a(n) = the n‑th integer k such that 11*k + 7 is prime
   (indexing starts at n = 1).  The implementation scans successive k
   values and counts primes until the desired rank is reached. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. For n == 0 return 0 as
       a harmless default (the caller is expected not to request it). */
    if (n == 0)
        return 0;

    idx_t count = 0;   /* how many suitable k's we have found so far */
    idx_t k = 0;       /* candidate k to test */

    while (1) {
        /* 11*k + 7 fits in uint64_t for all k that occur when n ≤ 10 000
           and well beyond the limits of the test harness. */
        uint64_t p = 11ULL * k + 7ULL;

        if (is_prime_u64(p)) {
            ++count;
            if (count == n)
                return (seq_t)k;   /* cast to seq_t; works for all supported types */
        }
        ++k;
    }
}
