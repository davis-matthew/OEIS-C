/*
 * A054099  Sum{T(n,k): k=0,1,...,n}, array T as in A054098.
 * https://oeis.org/A054099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=6, a(n) = (n-1)*a(n-1) + 2 for n ≥ 3 (sum
 *            of triangle A054098)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for incremental computation */
    static seq_t *table = NULL;
    static idx_t  last  = (idx_t)-1;   /* last index that is computed */
    static size_t cap   = 0;           /* allocated capacity (in elements) */

    const size_t MIN_CAP = 3;          /* need space for indices 0,1,2 */

    /* First call: allocate and initialise the base values */
    if (!table) {
        cap = (n + 1 > MIN_CAP) ? (n + 1) : MIN_CAP;
        table = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!table) return (seq_t)0;   /* allocation failed – unlikely in tests */
        table[0] = (seq_t)1;
        table[1] = (seq_t)3;
        table[2] = (seq_t)6;
        last = 2;
    }
    /* If the requested index exceeds current capacity, grow the table */
    else if ((size_t)(n + 1) > cap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(table, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* allocation failed */
        table = tmp;
        cap = newcap;
    }

    /* Return cached value if already computed */
    if (n <= last) return table[n];

    /* Compute values from last+1 up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* for i ≥ 3, recurrence holds; for i = 1,2 the values are already set */
        table[i] = (seq_t)(i - 1) * table[i - 1] + (seq_t)2;
    }
    last = n;
    return table[n];
}
