/*
 * A139104  Numbers whose binary representation shows the distribution of
 *          prime numbers up to the n-th prime, using "0" for primes and "1"
 *          for nonprime numbers.
 * https://oeis.org/A139104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A139104
    a(n) is the integer whose binary representation uses 0 for primes
    and 1 for composites from 1 up to the n‑th prime.  The first bit
    corresponds to 1, the second to 2, etc.  This function builds the
    binary string by sieving up to the n‑th prime on demand. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/_prime_limit_for_idx.h"   /* gets an upper bound on prime(n) */
#include "../utils/build_primes_upto.h"      /* builds an array of primes up to a limit */

seq_t a(idx_t n)
{
    /* static memoised data */
    static idx_t max_primes_needed = 0;   /* highest n processed so far */
    static idx_t prime_limit        = 0;   /* current sieve limit */
    static uint64_t *prime_list     = NULL;/* primes[] up to prime_limit   */
    static size_t   prime_count     = 0;   /* number of primes in prime_list */
    static uint8_t *prime_flag      = NULL;/* 1 = prime, 0 = composite, index 0..p */
    static idx_t    flag_limit      = 0;   /* largest index set in prime_flag */

    if (n == 0) /* the sequence is 1‑based; behaviour for n<1 is irrelevant */
        return 0;

    /* --------------------------------------------------------------------
       Ensure we have the first n primes available.
       -------------------------------------------------------------------- */
    if (n > max_primes_needed) {
        /* Get a safe upper bound for the n‑th prime. */
        idx_t new_limit = _prime_limit_for_idx(n);

        /* Allocate a fresh array of primes up to this limit. */
        uint64_t *tmp_primes = NULL;
        size_t   tmp_cnt     = 0;
        build_primes_upto(new_limit, &tmp_primes, &tmp_cnt);

        /* Replace old data. */
        free(prime_list);
        prime_list  = tmp_primes;
        prime_count = tmp_cnt;
        prime_limit = new_limit;
        max_primes_needed = n;
    }

    /* nth prime (1‑based index → 0‑based array). */
    idx_t p = prime_list[n - 1];

    /* --------------------------------------------------------------------
       Build a prime/composite flag array up to p if needed.
       -------------------------------------------------------------------- */
    if (p > flag_limit) {
        /* Allocate new flag array of size p+1 (indices 0..p). */
        uint8_t *new_flag = (uint8_t *)malloc((size_t)(p + 1));
        if (!new_flag)
            abort(); /* allocation failure */

        /* Initialise to 1 (prime) and clear 0,1. */
        for (idx_t i = 0; i <= p; ++i)
            new_flag[i] = 1;
        new_flag[0] = 0;
        new_flag[1] = 0;

        /* Sieve of Eratosthenes to mark composites. */
        for (idx_t i = 2; (i * i) <= p; ++i) {
            if (new_flag[i]) {
                for (idx_t j = i * i; j <= p; j += i)
                    new_flag[j] = 0;
            }
        }

        /* Replace old flag array. */
        free(prime_flag);
        prime_flag = new_flag;
        flag_limit = p;
    }

    /* --------------------------------------------------------------------
       Construct the binary number according to the specification.
       -------------------------------------------------------------------- */
    seq_t val = 0;
    for (idx_t i = 1; i <= p; ++i) {
        val <<= 1;                     /* shift left for next bit */
        if (!prime_flag[i])            /* composite → bit 1 */
            val |= 1;
    }

    return val;
}
