/*
 * A023790  Katadromes: digits in base 8 are in strict descending order.
 * https://oeis.org/A023790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, easy
 * Formula:   a(n) = n-th smallest non‑negative integer whose base‑8
 *            representation has strictly decreasing digits
 * Range:     u8 n <= 9, u16 n <= 163, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Comparator for qsort on seq_t values. */
static int seq_cmp(const void *p1, const void *p2)
{
    seq_t const *x = (seq_t const *)p1;
    seq_t const *y = (seq_t const *)p2;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Return the n-th term of A023790.  n is 1‑based; for n<1 or n>255
   the call is undefined, we conventionally return 0. */
seq_t a(idx_t n)
{
    static int ready = 0;
    static seq_t table[255];

    if (!ready)
    {
        /* Generate all numbers with strictly decreasing base‑8 digits. */
        for (idx_t mask = 1; mask <= 255; ++mask)
        {
            seq_t val = 0;
            for (int d = 7; d >= 0; --d)
            {
                if (mask & ((idx_t)1 << d))
                    val = val * (seq_t)8 + (seq_t)d;
            }
            table[mask - 1] = val;
        }
        /* Sort the values in increasing order. */
        qsort(table, 255, sizeof(seq_t), seq_cmp);
        ready = 1;
    }

    if (n == 0 || n > 255)
        return (seq_t)0;
    return table[n - 1];
}
