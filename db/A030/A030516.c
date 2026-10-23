/*
 * A030516  Numbers with 7 divisors. 6th powers of primes.
 * https://oeis.org/A030516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000040(n)^6 (prime(n) raised to the 6th power)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 257, u128 n <= 1000,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* Return the n‑th term of sequence A030516.
   a(1) = 64, a(2) = 729, a(3) = 15625, ...            */
seq_t a(idx_t n)
{
    /* Sequence defined only for n ≥ 1.  For n < 1 return 0. */
    if (n < 1) return (seq_t)0;

    /* Static storage for generated primes. */
    static uint64_t *primes = NULL;
    static size_t   primes_cnt = 0;
    static bool     initialized = false;
    static idx_t    limit = 200000;   /* sufficient for n ≈ 10000 */

    if (!initialized)
    {
        /* Build primes up to 'limit' once. */
        build_primes_upto(limit, &primes, &primes_cnt);
        initialized = true;
    }

    /* If the current table has too few primes, extend it. */
    if (primes_cnt < (size_t)n)
    {
        /* Double the limit until we have enough primes. */
        while (primes_cnt < (size_t)n)
        {
            limit *= 2;
            uint64_t *new_primes = NULL;
            size_t   new_cnt     = 0;
            build_primes_upto(limit, &new_primes, &new_cnt);
            /* Replace old array. */
            if (primes) free(primes);
            primes = new_primes;
            primes_cnt = new_cnt;
        }
    }

    /* Retrieve the n‑th prime (0‑based index n‑1). */
    uint64_t p = primes[n - 1];

    /* Compute p^6.  For arbitrary‑precision seq_t, use big_pow. */
    seq_t base   = (seq_t)p;
    seq_t result = big_pow(base, (seq_t)6);

    return result;
}
