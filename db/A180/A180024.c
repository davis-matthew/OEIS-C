/*
 * A180024  Smallest prime greater than n-th prime having as many ones in
 *          binary representation.
 * https://oeis.org/A180024
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime > prime(n) having the same number of
 *            ones in its binary representation
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6542, u64 n <= 6542,
 *            u128 n <= 6542, big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

/* Helper to count set bits of a seq_t value. Works for any unsigned
   integral type, including big integers. */
static inline idx_t popcount_seq(seq_t x)
{
    idx_t c = 0;
    while (x) {
        x &= x - (seq_t)1;
        ++c;
    }
    return c;
}

/* Static storage for primes.  The array is grown as necessary to
   contain at least `need` primes. */
static uint64_t *prime_array = NULL;
static size_t prime_count = 0;

/* Grow the prime table until it contains at least `need` primes. */
static void ensure_prime_table(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;                      /* already enough */

    /* We use the helper `_prime_limit_for_idx` to get an upper bound
       for the n-th prime.  We then build all primes up to that bound
       and, if necessary, double the bound until we have enough. */
    size_t limit = _prime_limit_for_idx(need);
    while (true) {
        uint64_t *new_array = NULL;
        size_t new_count = 0;
        build_primes_upto(limit, &new_array, &new_count);
        free(prime_array);
        prime_array = new_array;
        prime_count = new_count;
        if (prime_count >= (size_t)need)
            return;
        limit *= 2;                  /* too few primes – grow limit */
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;            /* undefined by the sequence definition */

    ensure_prime_table(n);          /* guarantees prime_array[n-1] exists */

    seq_t p = prime_array[n - 1];   /* n-th prime (0‑based index) */
    idx_t target = popcount_seq(p); /* popcount of the n-th prime */

    seq_t cand = p + (seq_t)1;      /* start searching from the next number */

    for (;;) {
        /* Only test primality for values that fit into idx_t.  For the
           typical range of the problem this is sufficient. */
        if ((idx_t)cand == cand && is_prime((idx_t)cand)) {
            if (popcount_seq(cand) == target)
                return cand;       /* found the required prime */
        }
        ++cand;
    }
}
