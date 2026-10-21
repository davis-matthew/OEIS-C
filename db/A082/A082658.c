/*
 * A082658  Integers expressible as the sum of a square and a triangular
 *          number in exactly two distinct ways.
 * https://oeis.org/A082658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes integers expressible as the sum of a square and a triangular
   number in exactly two distinct ways (OEIS A082658). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Minimum limit to start the search; can be increased later. */
static const idx_t START_LIMIT = 2000;

/* Cache of the sequence */
static seq_t *sequence = NULL;      /* array of terms */
static idx_t  seq_len  = 0;          /* number of terms stored */
static idx_t  limit_used = 0;        /* largest limit already processed */

/* Build the sequence up to at least 'needed' terms. */
static void build_until(idx_t needed)
{
    /* If we already have enough terms, nothing to do. */
    if (seq_len >= needed)
        return;

    /* Start with the previous limit, or with the START_LIMIT. */
    idx_t lim = (limit_used == 0) ? START_LIMIT : limit_used;

    while (true)
    {
        /* Allocate counting array for values 0 … lim. */
        uint16_t *cnt = (uint16_t *)calloc((size_t)(lim + 1), sizeof(uint16_t));
        if (!cnt)
            exit(1);

        /* Count representations as square + triangle. */
        for (idx_t x = 0; ; ++x)
        {
            idx_t sq = (idx_t)x * (idx_t)x;
            if (sq > lim)
                break;

            /* Triangular numbers t(y) = y*(y+1)/2, y >= 1. */
            for (idx_t y = 1;; ++y)
            {
                idx_t tri = (idx_t)y * (idx_t)(y + 1) / 2;
                idx_t sum = sq + tri;
                if (sum > lim)
                    break;
                /* Count this representation. */
                ++cnt[sum];
            }
        }

        /* Count how many numbers have exactly two representations. */
        idx_t valid = 0;
        for (idx_t v = 0; v <= lim; ++v)
            if (cnt[v] == 2)
                ++valid;

        /* Allocate new sequence array and fill it. */
        seq_t *new_seq = (seq_t *)malloc((size_t)valid * sizeof(seq_t));
        if (!new_seq)
            exit(1);

        idx_t pos = 0;
        for (idx_t v = 0; v <= lim; ++v)
            if (cnt[v] == 2)
                new_seq[pos++] = (seq_t)v;

        free(cnt);

        /* Replace old cache. */
        free(sequence);
        sequence = new_seq;
        seq_len   = valid;
        limit_used = lim;

        /* If we now have enough terms, stop. */
        if (seq_len >= needed)
            return;

        /* Not enough terms yet – double the limit and try again. */
        lim *= 2;
    }
}

/* Public function: return the n-th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for n = 0 */

    build_until(n);
    return sequence[(size_t)(n - 1)];
}
