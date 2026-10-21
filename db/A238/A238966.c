/*
 * A238966  The number of distinct primes in divisor lattice in canonical
 *          order.
 * https://oeis.org/A238966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of parts of the n‑th integer partition * in graded
 *            reverse‑lexicographic order. a(0) = 0
 * Range:     u8 n <= 2713, u16 n <= 2713, u32 n <= 2713, u64 n <= 2713,
 *            u128 n <= 2713, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, abort */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Allocate space for the current partition. */
    size_t sz = (size_t)n + 1;          /* enough for the worst case n ones */
    int *part = (int *)malloc(sz * sizeof(int));
    if (!part)
        abort();                       /* out of memory */

    int current_sum = 1;                /* sum of the parts (rank 1) */
    int part_len    = 1;                /* number of parts in current partition */
    part[0] = 1;

    idx_t rank = 1;                     /* current partition rank (1‑based) */
    if (n == 1) {                       /* first partition [1] */
        free(part);
        return 1;
    }

    while (rank < n)
    {
        /* Find rightmost part > 1. */
        int j = part_len - 1;
        while (j >= 0 && part[j] == 1)
            j--;

        if (j < 0)                      /* all ones → next sum */
        {
            ++current_sum;
            part[0] = current_sum;
            part_len = 1;
        }
        else
        {
            /* Decrease that part by one. */
            --part[j];

            /* Compute sum of parts up to j. */
            int used = 0;
            for (int t = 0; t <= j; ++t)
                used += part[t];

            int rem = current_sum - used;   /* remaining value to distribute */
            int k = j + 1;

            /* Replicate part[j] as many times as possible. */
            while (rem > part[j])
            {
                part[k++] = part[j];
                rem -= part[j];
            }

            /* Final remainder. */
            part[k] = rem;
            part_len = k + 1;
        }

        ++rank;
    }

    seq_t result = (seq_t)part_len;
    free(part);
    return result;
}
