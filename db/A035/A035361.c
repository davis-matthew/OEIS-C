/*
 * A035361  Number of partitions of n into parts 3k or 3k+2.
 * https://oeis.org/A035361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts congruent to 0 or 2
 *            mod 3
 * Range:     u8 n <= 29, u16 n <= 72, u32 n <= 205, u64 n <= 650,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Memoized DP table */
static seq_t *part_tbl = NULL;
static idx_t   max_n   = 0;

/* Main function returning the nth term */
seq_t a(idx_t n) {
    /* Allocate and compute DP up to n if not done yet */
    if (part_tbl == NULL || n > max_n) {
        idx_t new_size = n + 1;
        seq_t *new_tbl = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (new_tbl == NULL)
            return 0; /* error: out of memory */
        /* zero initialise */
        for (idx_t i = 0; i < new_size; ++i)
            new_tbl[i] = 0;
        new_tbl[0] = 1;  /* empty partition */

        /* Dynamic programming over allowed part sizes 2,3,5,6,… (≠1 mod 3) */
        for (idx_t s = 2; s <= n; ++s) {
            if (s % 3 == 1) continue;           /* skip forbidden sizes */
            for (idx_t i = s; i <= n; ++i)
                new_tbl[i] += new_tbl[i - s];
        }

        /* replace old table */
        if (part_tbl != NULL)
            free(part_tbl);
        part_tbl = new_tbl;
        max_n   = n;
    }

    return part_tbl[n];
}
