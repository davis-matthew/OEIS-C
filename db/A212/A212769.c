/*
 * A212769  p*q modulo (p+q) with p, q consecutive primes.
 * https://oeis.org/A212769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_n * p_{n+1}) mod (p_n + p_{n+1}), where p_k is the
 *            k‑th prime
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS sequence A212769. */
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"   /* _prime_limit_for_idx() */
#include "../utils/build_primes_upto.h"      /* build_primes_upto() */

seq_t a(idx_t n)
{
    /* OEIS offset: a(1) is the first term.  For n < 1 we return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache of consecutive primes. */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt   = 0;

    /* We need primes with indices 0..n (i.e., n+1 primes). */
    idx_t required = n + 1;
    if (prime_cnt < (size_t)required) {
        /* Determine a limit that guarantees at least required primes.
           Add a safety margin of 10. */
        idx_t limit = _prime_limit_for_idx(required) + 10;

        /* Rebuild the prime list up to the new limit. */
        uint64_t *new_list = NULL;
        size_t     new_cnt  = 0;
        build_primes_upto(limit, &new_list, &new_cnt);

        /* Replace the old list (if any). */
        free(prime_list);
        prime_list = new_list;
        prime_cnt   = new_cnt;
    }

    /* Retrieve the consecutive primes. */
    uint64_t p = prime_list[n - 1];
    uint64_t q = prime_list[n];

    /* Compute (p * q) % (p + q) using 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 prod   = (unsigned __int128)p * q;
    unsigned __int128 denom  = (unsigned __int128)p + q;
    unsigned __int128 mod    = prod % denom;

    return (seq_t)mod;
}
