/*
 * A088134  Numbers n such that sum of first and last digits is prime.
 * https://oeis.org/A088134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = the n-th natural number N such that the sum of the first
 *            and last decimal digits of N is a prime number. The sequence is
 *            indexed from 1
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   sum_is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sum_is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, just return 0 */

    /* Cached terms */
    static seq_t *terms = NULL;
    static idx_t terms_len = 0, terms_cap = 0;
    static idx_t next_candidate = 1;

    /* Generate terms until we have the n-th term */
    while (terms_len < n)
    {
        /* Test the current candidate */
        idx_t cand = next_candidate;
        idx_t last = cand % 10;
        idx_t first = cand;
        while (first >= 10) first /= 10;
        int sum = (int)first + (int)last;

        if (sum_is_prime(sum))
        {
            /* Store the found term */
            if (terms_cap - terms_len < 1)    /* grow capacity */
            {
                idx_t new_cap = terms_cap ? terms_cap * 2 : 16;
                terms = (seq_t *)realloc(terms, sizeof(seq_t) * new_cap);
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)cand;
        }

        /* Advance to next candidate */
        next_candidate = cand + 1;
    }

    return terms[n - 1];
}
