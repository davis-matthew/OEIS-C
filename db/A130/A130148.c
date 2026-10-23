/*
 * A130148  a(1) = a(2) = 1. For n > 1, n is repeated (a(n)+a(n-1)) times.
 * https://oeis.org/A130148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1965, u16 n <= 1965, u32 n <= 1965, u64 n <= 1965,
 *            u128 n <= 1965, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A130148
   a(1) = a(2) = 1.
   For n > 1, the integer n occurs exactly (a(n)+a(n‑1)) times in the sequence.
   The values a(n) are the terms of the sequence (index‑wise). */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special‑case the defined start. */
    if (n <= 2)
        return 1;

    /* Allocate an array for indices 1 … n (index 0 unused). */
    /* size_t is guaranteed to hold the count n+1 for n up to 10000. */
    seq_t *seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq)
        return 0;      /* allocation failure – undefined for tests */

    /* Initialise the known first terms. */
    seq[1] = 1;
    seq[2] = 1;

    /* Current position to fill.
       k points to the integer whose run we are generating. */
    idx_t pos = 3;   /* next free index (1‑based) */
    idx_t k   = 2;   /* integer being processed */

    while (pos <= n) {
        /* How many times integer k must appear. */
        idx_t run_len = (idx_t)seq[k] + (idx_t)seq[k - 1];

        /* If the run extends beyond the requested index n, trim it. */
        idx_t remaining = n - pos + 1;
        if (remaining < run_len)
            run_len = remaining;

        /* Fill the run with the value k. */
        for (idx_t i = 0; i < run_len; ++i)
            seq[pos + i] = (seq_t)k;

        pos += run_len;
        ++k;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
