/*
 * A069028  Powers of 3 with strictly increasing sum of digits.
 * https://oeis.org/A069028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A069028: powers of 3 whose digit sum is strictly larger than all
 * preceding such digit sums.  No closed formula is known; we generate
 * terms incrementally by checking successive powers of 3.  The
 * sequence is stored in a static array so that random accesses work
 * efficiently. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"   /* declares seq_t digit_sum(seq_t) */

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* array of discovered terms          */
    static size_t   terms_len = 0;/* how many terms are stored yet      */
    static size_t   terms_cap = 0;/* current allocation capacity        */

    static seq_t power    = (seq_t)1; /* current 3^k (starting with 3^0)   */
    static seq_t max_sum  = (seq_t)0; /* maximum digit sum seen so far      */

    /* Ensure space for the requested index */
    while ((size_t)terms_len < (size_t)n)
    {
        /* If we need to store a new term, grow the array if necessary */
        if (terms_len == terms_cap)
        {
            size_t new_cap = terms_cap == 0 ? 16 : terms_cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            /* realloc may fail; the test harness assumes enough memory */
            if (!tmp) return (seq_t)0; /* graceful failure, unlikely in tests */
            terms      = tmp;
            terms_cap  = new_cap;
        }

        /* Compute digit sum of current power of 3 */
        seq_t sum = digit_sum(power);

        /* If this sum is strictly larger, record the term */
        if (sum > max_sum)
        {
            terms[terms_len++] = power;
            max_sum = sum;
        }

        /* Advance to the next power of 3 */
        power = power * (seq_t)3;
    }

    return terms[n - 1];
}
