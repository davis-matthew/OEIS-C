/*
 * A174665  Sequence whose indices are based on the Fibonacci sequence as
 *          explained in comments.
 * https://oeis.org/A174665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A174665
 * a(0) = 1; a(1) = 2.
 * For n ≥ 2:
 *   a(F(n) − 1) = (3^(n−2) + 3)/2   where F is the Fibonacci sequence
 *   (F(0)=1, F(1)=1, F(2)=2, …).
 * Between two special indices F(m) − 1 and F(m+1) − 1, the block of
 * terms is a copy of the first F(m) − 1 terms of the sequence.
 */

#include <stdlib.h>

/* External function that returns a(n) for any n ≥ 0.
 * seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate array for the terms up to n. */
    seq_t *seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!seq)
        return (seq_t)0;       /* allocation failure – unlikely in tests */

    seq[0] = (seq_t)1;          /* base value */

    /* Fibonacci variables for generating the differences between
     * successive special indices.  fib_prev = F(k), fib_curr = F(k+1). */
    idx_t fib_prev = (idx_t)1;   /* F(1) */
    idx_t fib_curr = (idx_t)1;   /* F(2) */
    idx_t diff      = fib_prev;  /* current difference; first is 1 */
    idx_t next_special = diff;   /* first special index after 0 */

    idx_t prev_special     = (idx_t)0;   /* last processed special index */
    seq_t prev_special_val = (seq_t)1;   /* a(prev_special) */

    for (idx_t i = 1; i <= n; ++i)
    {
        if (i == next_special)              /* special index */
        {
            if (i == (idx_t)1)              /* a(1) is 2, special case */
                seq[i] = (seq_t)2;
            else
                seq[i] = (seq_t)3 * prev_special_val - (seq_t)3;

            prev_special_val = seq[i];
            prev_special     = i;

            /* Advance Fibonacci numbers to compute next difference. */
            idx_t fib_next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = fib_next;
            diff      = fib_prev;          /* next difference = new F(k) */
            next_special = i + diff;       /* index of next special */
        }
        else                                  /* replicated block */
        {
            seq[i] = seq[i - prev_special - (idx_t)1];
        }
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
