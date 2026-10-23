/*
 * A105828  For n>2, a(n) not appeared previously is such that
 *          a(n-1)^2+4*a(n-2)*a(n) is a minimal square, a(1)=1, a(2)=3.
 * https://oeis.org/A105828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A105828:  a(1)=1, a(2)=3.  For n>2, a(n) is the
   smallest positive integer not yet in the sequence such that
   a(n-1)^2 + 4*a(n-2)*a(n) is a perfect square (choosing the
   minimal possible square). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_square.h"

/* Persistent storage for the sequence. */
static seq_t *seq = NULL;      /* array of computed terms   */
static idx_t seq_len = 0;      /* number of terms stored   */
static idx_t seq_cap = 0;      /* current allocation size  */

/* Ensure that the array can hold at least new_cap elements. */
static void ensure_cap(idx_t new_cap)
{
    if (new_cap <= seq_cap)
        return;
    if (new_cap == 0)
        new_cap = 2;                 /* minimal initial size   */
    else if (new_cap < seq_cap * 2)
        new_cap = seq_cap * 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Direct returns for the first two terms. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Initialise the sequence storage on first use. */
    if (seq_len == 0)
    {
        ensure_cap(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)3;
        seq_len = 2;
    }

    /* Compute terms up to the requested index. */
    while (seq_len < n)
    {
        seq_t p = seq[seq_len - 1];   /* a(n-1) */
        seq_t q = seq[seq_len - 2];   /* a(n-2) */
        seq_t cand;                  /* candidate for a(n) */

        for (cand = 1; ; ++cand)
        {
            /* Compute p^2 + 4*q*cand and test for a perfect square. */
            seq_t sum = p * p + (seq_t)4 * q * cand;
            if (!is_square(sum))
                continue;

            /* Ensure cand has not appeared before. */
            bool used = false;
            for (idx_t j = 0; j < seq_len; ++j)
            {
                if (seq[j] == cand)
                {
                    used = true;
                    break;
                }
            }
            if (!used)
                break;
        }

        /* Append the found term. */
        ensure_cap(seq_len + 1);
        seq[seq_len++] = cand;
    }

    return seq[n - 1];
}
