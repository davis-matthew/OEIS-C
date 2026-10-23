/*
 * A119261  Numbers with even decimal digits in decreasing order.
 * https://oeis.org/A119261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, full, nonn
 * Formula:   Enumerate all numbers composed of a subset of the even digits
 *            {0,2,4,6,8} with digits strictly decreasing; sort ascending;
 *            a(n) is the nth term (1‑based)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static table containing all 31 terms of the sequence. */
static seq_t seq[31];
static int initialized = 0;
static int table_size = 0;

/* Comparison function for qsort (ascending). */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Initialize the table on first call. */
    if (!initialized) {
        const unsigned char evens[5] = {0, 2, 4, 6, 8};
        int count = 0;
        for (idx_t mask = 1; mask < ((idx_t)1 << 5); ++mask) {
            seq_t val = (seq_t)0;
            /* Build the number with digits in decreasing order. */
            for (int d = 4; d >= 0; --d) {
                if (mask & ((idx_t)1 << d)) {
                    val = val * (seq_t)10 + (seq_t)evens[d];
                }
            }
            seq[count++] = val;
        }
        table_size = count;
        qsort(seq, (size_t)count, sizeof(seq_t), cmp_seq);
        initialized = 1;
    }

    if (n == 0 || n > (idx_t)table_size)
        return (seq_t)0;      /* undefined for out‑of‑range indices */
    return seq[(idx_t)n - 1];
}
