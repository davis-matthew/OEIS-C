/*
 * A200917  Primes of the form 7n^3-3.
 * https://oeis.org/A200917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime of the form 7k^3-3 (OEIS A200917)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 42 terms through n = 972 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n=0, but safe fallback */

    idx_t  k    = 1;               /* current k (k >=1) */
    idx_t  count = 0;             /* number of primes found so far */

    while (1) {
        /* Compute 7*k^3 - 3 in a wide intermediate to avoid overflow */
        unsigned __int128 tk = k;
        unsigned __int128 val = (unsigned __int128)7 * tk * tk * tk; /* 7*k^3 */
        val -= 3;                                      /* subtract 3 */

        seq_t candidate = (seq_t)val;                  /* cast to seq_t */

        if (is_prime_seq(candidate)) {                 /* test primality */
            ++count;
            if (count == n) return candidate;         /* found n-th prime */
        }
        ++k;
    }
}
