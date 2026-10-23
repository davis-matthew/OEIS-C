/*
 * A140373  Primes of the form 26*n+11.
 * https://oeis.org/A140373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140373: primes of the form 26*n + 11 (n ≥ 0; a(1) = 11) */
/* The function returns the n-th prime in that progression (n ≥ 1).  It
 * scans the arithmetic progression 26*k+11 and tests each candidate
 * for primality using the provided is_prime(idx_t) helper, which is
 * efficient for numbers that fit in 64 bits.  Because all candidates
 * stay well below 2³² for the requested range, the routine is fast
 * enough for n up to 10 000. */
 
#include <stdbool.h>
#include "../utils/is_prime.h"
 
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined below offset, guard clause */
 
    idx_t found = 0;
    idx_t k = 0;
    for (;; ++k) {
        seq_t candidate = ((seq_t)k) * 26 + 11;   /* 26*k + 11 */
        if (is_prime((idx_t)candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
    }
}
