/*
 * A182376  Smallest k>0 such that n-k, n+k, 3n-k and 3n+k are all primes, or
 *          0 if no such k exists.
 * https://oeis.org/A182376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = smallest k>0 such that n-k, n+k, 3n-k and 3n+k are all
 *            primes, or 0 if no such k exists
 * Range:     u8 n <= 275, u16 n <= 9105, u32 n <= 9105, u64 n <= 9105,
 *            u128 n <= 9105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;                /* no primes can be formed */

    /* 3*n may overflow uint64_t for very large n.  Since our prime test
       is defined only for idx_t (uint64_t), we bail out if 3*n would
       overflow, as we cannot correctly test primality beyond that limit. */
    unsigned __int128 triple = (unsigned __int128)n * 3;
    if (triple > UINT64_MAX) return (seq_t)0;

    /* Search for the smallest k.  k must be < n to keep n-k positive. */
    for (idx_t k = 1; k < n; ++k) {
        idx_t n_minus_k = n - k;               /* n-k is guaranteed >= 1 */
        if (n_minus_k < 2) continue;           /* 0 or 1 is not prime */

        if (!is_prime(n_minus_k)) continue;

        idx_t n_plus_k = n + k;
        if (!is_prime(n_plus_k)) continue;

        unsigned __int128 c_val = triple - k;  /* 3n-k */
        if (c_val < 2) continue;               /* cannot be prime */
        if (!is_prime((idx_t)c_val)) continue;

        unsigned __int128 d_val = triple + k;  /* 3n+k */
        if (!is_prime((idx_t)d_val)) continue;

        /* All four numbers are prime, return the k found. */
        return (seq_t)k;
    }
    return (seq_t)0;                           /* no suitable k exists */
}
