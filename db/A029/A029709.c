/*
 * A029709  Numbers k such that k-th and (k+1)-th primes differ by 4.
 * https://oeis.org/A029709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where the k‑th and (k+1)‑th primes differ by 4
 * Range:     u8 n <= 57, u16 n <= 6932, u32 n <= 8825, u64 n <= 8825,
 *            u128 n <= 8825, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A029709 – Positions of 4 in the prime‑gap sequence. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Determine the number of prime gaps of size 4 up to a given sieve limit
   and store their indices (1‑based) in a static array.  The sieve limit
   is doubled until at least n gaps are found.  The routine keeps its
   internal state between calls, so it works for arbitrary call order. */

seq_t a(idx_t n)
{
    /* Guard against accidental zero.  The sequence is 1‑based. */
    if (n == 0)
        return 0;

    /* Static state preserved between calls. */
    static size_t current_limit   = 0;          /* last sieve limit used    */
    static uint64_t *primes       = NULL;      /* list of primes up to limit */
    static size_t primes_count    = 0;          /* number of primes           */
    static seq_t  *gap_indices    = NULL;      /* indices k where gaps=4     */
    static size_t gaps_count      = 0;          /* number of gaps found       */

    /* Sieve until we have at least n gaps. */
    while (gaps_count < (size_t)n)
    {
        /* Choose a new sieve limit.  Start reasonably large, then
           double each time we need more data. */
        size_t new_limit = (current_limit == 0) ? 200000 : current_limit * 2;
        if (new_limit < 200000)
            new_limit = 200000;

        /* Allocate and initialise a composite flag array. */
        uint8_t *is_composite =
            (uint8_t *)malloc(new_limit + 1);      /* always a multiple of 1 byte */
        if (is_composite == NULL)
            return 0;                              /* out of memory – abort */
        memset(is_composite, 0, new_limit + 1);

        /* Simple Eratosthenes sieve. */
        for (size_t i = 2; i * i <= new_limit; ++i)
            if (!is_composite[i])
                for (size_t j = i * i; j <= new_limit; j += i)
                    is_composite[j] = 1;

        /* First pass: count primes. */
        size_t count = 0;
        for (size_t i = 2; i <= new_limit; ++i)
            if (!is_composite[i])
                ++count;

        /* Allocate or resize the primes array. */
        uint64_t *tmp_primes = (uint64_t *)realloc(primes, count * sizeof(uint64_t));
        if (tmp_primes == NULL) {
            free(is_composite);
            return 0;                              /* out of memory – abort */
        }
        primes = tmp_primes;
        primes_count = count;

        /* Second pass: collect primes. */
        size_t idx = 0;
        for (size_t i = 2; i <= new_limit; ++i)
            if (!is_composite[i])
                primes[idx++] = (uint64_t)i;

        /* Release the composite array. */
        free(is_composite);

        /* Compute all gaps of size 4 in the new prime list. */
        size_t new_gaps = 0;
        for (size_t k = 0; k + 1 < primes_count; ++k)
            if (primes[k + 1] - primes[k] == 4)
                ++new_gaps;

        /* Allocate or resize the gap_indices array to hold all gaps. */
        seq_t *tmp_gaps = (seq_t *)realloc(gap_indices, new_gaps * sizeof(seq_t));
        if (tmp_gaps == NULL) {
            /* Non‑fatal – keep the old array around. */
            return 0;                          /* out of memory – abort */
        }
        gap_indices = tmp_gaps;

        /* Store the 1‑based indices of the gaps. */
        size_t pos = 0;
        for (size_t k = 0; k + 1 < primes_count; ++k)
            if (primes[k + 1] - primes[k] == 4)
                gap_indices[pos++] = (seq_t)(k + 1);

        /* Update the static counters and limit. */
        gaps_count = new_gaps;
        current_limit = new_limit;

        /* If we still haven't found enough gaps, the loop will iterate
           again with a larger limit. */
    }

    /* Return the (n‑1)‑th element (zero‑based indexing). */
    return gap_indices[(size_t)n - 1];
}
