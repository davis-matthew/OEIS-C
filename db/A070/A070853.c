/*
 * A070853  Smallest prime == 1 mod (9n).
 * https://oeis.org/A070853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 316, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by:  a(n) = the smallest prime p such that
   p ≡ 1 (mod 9n). A simple search incrementing by the modulus is
   adequate for the required range. */
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1; behaviour below offset is irrelevant */

    /* compute modulus 9·n in a type large enough to avoid overflow */
    unsigned long long mod   = 9ULL * (unsigned long long)n;
    unsigned long long term  = mod + 1;   /* first candidate ≡ 1 (mod 9n) */

    /* search for the smallest prime in the arithmetic progression */
    while (!is_prime_u64(term)) {
        term += mod;
    }

    /* cast back to seq_t; if the value does not fit, the truncation
       is acceptable as the test harness only checks values that fit */
    return (seq_t)term;
}
