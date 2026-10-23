/*
 * A153165  Primes of form 6k+1 that use only digits 2 and 3.
 * https://oeis.org/A153165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 4, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes of form 6k+1 consisting solely of the digits 2 and 3. */
/* The algorithm enumerates all numbers with digits 2/3 in increasing numeric
   order, tests each for 6k+1 and primality, and caches the results so that
   repeated calls to any position are fast. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cached list of found primes */
    static seq_t *primes = NULL;   /* array of primes */
    static size_t primes_cnt = 0;  /* number of primes stored */
    static size_t primes_cap = 0;  /* allocated capacity */

    /* Static state for enumerating candidates */
    static idx_t cur_len   = 1;        /* current number of decimal digits   */
    static uint64_t cur_off = 0ULL;    /* offset within all patterns of cur_len */
    static uint64_t cur_limit = 2ULL;  /* number of patterns = 2^cur_len     */

    /* Ensure the primes array has room for at least one more prime. */
    if (primes_cnt + 1 > primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* allocation failure */
        primes     = tmp;
        primes_cap = new_cap;
    }

    /* Enumerate candidates until we have at least n primes. */
    while (primes_cnt < (size_t)n) {
        /* Build the candidate number from cur_len digits (2 or 3). */
        seq_t num = 0;
        for (idx_t pos = 0; pos < cur_len; ++pos) {
            int bit = (int)((cur_off >> (cur_len - 1 - pos)) & 1ULL);
            num = num * 10 + (bit ? 3 : 2);
        }

        /* Test for the 6k+1 property and primality. */
        if ((num % 6) == 1 && is_prime_big(num)) {
            /* Store the found prime. */
            primes[primes_cnt++] = num;
        }

        /* Advance to the next pattern. */
        ++cur_off;
        if (cur_off == cur_limit) {    /* finished all patterns of this length */
            ++cur_len;
            cur_off  = 0;
            if (cur_len < 64)          /* safe shift: we never need more than 63 bits */
                cur_limit <<= 1;
        }
    }

    return primes[n - 1];   /* sequence is 1‑indexed */
}
