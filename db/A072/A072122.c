/*
 * A072122  Numbers with 12 odd integers in their Collatz (or 3x+1)
 *          trajectory.
 * https://oeis.org/A072122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 170, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose Collatz trajectory contains exactly 12 odd integers. */
#include <stdlib.h>
#include <stdint.h>

/* Computes a(n): the n‑th integer whose Collatz trajectory has exactly
 * 12 odd values.  The implementation repeatedly tests successive
 * integers until the required quantity has been found, memoising the
 * results so that calls are not re‑computed. */
seq_t a(idx_t n)
{
    static seq_t *terms      = NULL;   /* cached terms with 12 odd entries */
    static size_t terms_cap  = 0;      /* capacity of the cache */
    static size_t terms_len  = 0;      /* number of terms stored */
    static idx_t  cur = 1;            /* next candidate integer to test */

    /* Keep generating terms until we have at least n of them. */
    while (terms_len < n)
    {
        /* Ensure enough space in the cache. */
        if (terms_len + 1 > terms_cap)
        {
            size_t new_cap = terms_cap ? (terms_cap << 1) : 4;
            seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            terms = tmp;
            terms_cap = new_cap;
        }

        idx_t candidate = cur++;
        seq_t x = (seq_t)candidate;
        uint8_t odd_count = 0;

        /* Compute the Collatz trajectory of candidate and count odds. */
        while (1)
        {
            if (x & 1)
            {
                odd_count++;
                if (odd_count > 12) /* no need to continue */
                    break;
            }

            if (x == 1)          /* reached the terminating 1 */
                break;

            /* Next step of the Collatz function. */
            if (x & 1)
                x = (seq_t)3 * x + 1;
            else
                x >>= 1;
        }

        /* If exactly 12 odd numbers were found, cache this candidate. */
        if (odd_count == 12)
            terms[terms_len++] = (seq_t)candidate;
    }

    /* Return the n‑th term (1‑based). */
    return terms[n - 1];
}
