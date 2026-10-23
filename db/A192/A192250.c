/*
 * A192250  0-sequence of reduction of central binomial coefficient sequence
 *          by x^2 -> x+1.
 * https://oeis.org/A192250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   (n-1)(n-2)a(n) -(5n-7)(n-2)a(n-1) - 2(2n-3)(3n-8)a(n-2) +
 *            4(2n-3)(2n-5)a(n-3)=0 (n ≥ 4) Initial values: a(1)=1, a(2)=1,
 *            a(3)=7
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, exit */

static seq_t *seq = NULL;          /* memoised values, 1‑based */
static idx_t  seq_cap = 0;         /* allocated array size */
static idx_t  max_computed = 0;    /* highest n already computed */

/* Make sure the internal array can hold indices up to “up_to” (inclusive) */
static void ensure_capacity(idx_t up_to)
{
    if (seq_cap > up_to)
        return;

    /* double until we have enough space */
    idx_t new_cap = seq_cap ? seq_cap << 1 : 4;
    while (new_cap <= up_to)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    seq = tmp;
    seq_cap = new_cap;
}

/* Return the nth term of the sequence, computing and memoising as required */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* unused index */

    /* Ensure the buffer is large enough for index n */
    ensure_capacity(n);

    /* Compute any missing values up to n */
    while (max_computed < n)
    {
        idx_t k = max_computed + 1;
        seq_t val;

        if (k == 1)
            val = 1;
        else if (k == 2)
            val = 1;
        else if (k == 3)
            val = 7;
        else
        {
            /* Compute the recurrence term by term.  All intermediates
               are held in seq_t, so arbitrary‑precision builds are
               handled correctly. */
            seq_t term1 = (seq_t)(5 * k - 7) * (seq_t)(k - 2) * seq[k - 1];

            seq_t term2 = (seq_t)2 * (seq_t)(2 * k - 3) *
                          (seq_t)(3 * k - 8) * seq[k - 2];

            seq_t term3 = (seq_t)4 * (seq_t)(2 * k - 3) *
                          (seq_t)(2 * k - 5) * seq[k - 3];

            seq_t numerator = term1 + term2;
            numerator -= term3;

            seq_t denom = (seq_t)(k - 1) * (seq_t)(k - 2);

            val = numerator / denom;
        }

        seq[k] = val;
        max_computed = k;
    }

    return seq[n];
}
