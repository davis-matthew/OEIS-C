/*
 * A027055  a(n) = T(n, n+4), T given by A027052.
 * https://oeis.org/A027055
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) -5*a(n-2) +2*a(n-3) -a(n-4) +2*a(n-5) -a(n-6)
 *            for n>9 Initial values: a(4)=1, a(5)=18, a(6)=59, a(7)=146,
 *            a(8)=319, a(9)=652
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 34, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Behaviour for indices below the offset is irrelevant. */
    if (n < 4)
        return (seq_t)0;

    /* Static cache for computed terms. */
    static seq_t *table = NULL;
    static size_t cap   = 0;      /* allocated size of table */
    static size_t max_comp = 0;   /* largest index already computed */

    /* Ensure table is large enough for requested index. */
    size_t idx = (size_t)n;
    if (cap <= idx) {
        size_t newcap = idx + 1;
        if (newcap < 10)  /* room for initial values */
            newcap = 10;
        seq_t *tmp = (seq_t *)realloc(table, newcap * sizeof(seq_t));
        if (!tmp)         /* out of memory: return 0 as a fallback */
            return (seq_t)0;
        table = tmp;
        for (size_t i = cap; i < newcap; ++i)
            table[i] = (seq_t)0;
        cap = newcap;
    }

    /* Initialise the first nine values once. */
    if (max_comp < 9) {
        table[4] = (seq_t)1;
        table[5] = (seq_t)18;
        table[6] = (seq_t)59;
        table[7] = (seq_t)146;
        table[8] = (seq_t)319;
        table[9] = (seq_t)652;
        max_comp = 9;
    }

    /* Compute missing values up to the requested index. */
    if (max_comp < idx) {
        for (size_t i = max_comp + 1; i <= idx; ++i) {
            table[i] = (seq_t)4 * table[i - 1];
            table[i] -= (seq_t)5 * table[i - 2];
            table[i] += (seq_t)2 * table[i - 3];
            table[i] -= table[i - 4];
            table[i] += (seq_t)2 * table[i - 5];
            table[i] -= table[i - 6];
        }
        max_comp = idx;
    }

    return table[idx];
}
