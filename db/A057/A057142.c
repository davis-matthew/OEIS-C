/*
 * A057142  Number of occurrences of the most frequently occurring number in a
 *          1-to-n multiplication table.
 * https://oeis.org/A057142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum number of ordered pairs (i,j) with 1≤i,j≤n such
 *            that i·j equals a fixed value, i.e. the largest multiplicity in
 *            the n×n multiplication table
 * Range:     u8 n <= 9601, u16 n <= 9601, u32 n <= 9601, u64 n <= 9601,
 *            u128 n <= 9601, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static frequency table reused across calls. */
static uint16_t *freq_table = NULL;
static size_t      freq_capacity = 0;

/* Ensure the frequency table is large enough to hold indices up to n². */
static void ensure_freq_capacity(size_t needed)
{
    if (freq_capacity < needed) {
        uint16_t *tmp = (uint16_t *)realloc(freq_table,
                                           needed * sizeof(uint16_t));
        if (!tmp) abort();        /* allocation failure */
        freq_table = tmp;
        freq_capacity = needed;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥1 by the problem statement. */
    if (n == (idx_t)0) return (seq_t)0;  /* safety guard */

    /* Convert to size_t for loop bounds; 64‑bit is enough for n up to 10⁴. */
    size_t N = (size_t)n;
    size_t needed = N * N + 1;   /* indices 0 … N² needed */

    ensure_freq_capacity(needed);
    /* Clear only the part of the table that will be used. */
    memset(freq_table, 0, needed * sizeof(uint16_t));

    uint16_t max_occ = 0;
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = 1; j <= N; ++j) {
            size_t prod = i * j;         /* product fits into size_t */
            uint16_t cnt = ++freq_table[prod];
            if ((uint16_t)cnt > max_occ) max_occ = cnt;
        }
    }
    return (seq_t)max_occ;
}
