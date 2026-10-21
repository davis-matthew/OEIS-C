/*
 * A067975  a(n+1) is the smallest number > a(n) such that the digits of
 *          a(n)^2 are all (with multiplicity) contained in the digits of
 *          a(n+1)^2, with a(0)=2.
 * https://oeis.org/A067975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 24, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=2, a(n+1) = smallest integer > a(n) whose square contains
   all digits (with multiplicity) of a(n)^2.  The function memoises the
   sequence so that any index may be requested in any order. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_frequencies.h"

#define MAX_TERMS 20000          /* sufficient for n ≤ 10000 in tests */

static seq_t terms[MAX_TERMS];   /* table of computed terms */
static size_t terms_len = 0;     /* number of stored terms */

/* Initialise the table on first use. */
static void init_terms(void)
{
    if (terms_len != 0)
        return;
    terms[0] = (seq_t)2;
    terms_len = 1;
}

/* Compute the nth term, extending the table as necessary. */
seq_t a(idx_t n)
{
    init_terms();

    /* Extend the table to index n. */
    while (terms_len <= (size_t)n)
    {
        seq_t curr = terms[terms_len - 1];
        seq_t curr_sq = curr * curr;

        unsigned int currFreq[10] = {0};
        digit_frequencies(curr_sq, currFreq);

        seq_t cand = curr + (seq_t)1;
        for (;;)
        {
            seq_t cand_sq = cand * cand;
            unsigned int candFreq[10] = {0};
            digit_frequencies(cand_sq, candFreq);

            bool ok = true;
            for (int d = 0; d < 10; ++d)
            {
                if (currFreq[d] > candFreq[d])
                {
                    ok = false;
                    break;
                }
            }
            if (ok)
            {
                terms[terms_len] = cand;
                terms_len++;
                break;
            }
            cand++;
        }
    }

    return terms[n];
}
