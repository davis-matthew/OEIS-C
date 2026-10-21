/*
 * A078346  a(1) = 1; a(n) = Sum_{k=1..n-1} a(floor((n-1)/k)).
 * https://oeis.org/A078346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1}^{n-1} a(floor((n-1)/k)), a(1) = 1 (OEIS
 *            A078346). The sequence is memoised so out‑of‑order calls are
 *            fast
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *table      = NULL;   /* holds a(1) … a(max_computed) */
    static idx_t  capacity   = 0;     /* allocated size (index 0 ... capacity-1) */
    static idx_t  max_computed = 0;   /* highest index that has been filled */

    if (n < 1) return (seq_t)0;   /* safety: the sequence is defined for n≥1 */

    /* ensure the table can hold index n (index 0 is unused, but allocated) */
    if (capacity <= n) {
        idx_t newcap = capacity ? capacity << 1 : 2;
        while (newcap <= n) newcap <<= 1;
        void *tmp = realloc(table, newcap * sizeof(seq_t));
        table = (seq_t *)tmp;      /* cast required for C++ compilation */
        capacity = newcap;
    }

    /* initialise a(1) if necessary */
    if (max_computed < 1) {
        table[1] = (seq_t)1;
        max_computed = 1;
    }

    /* compute missing values up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        idx_t m = i - 1;
        seq_t sum = (seq_t)0;
        /* iterate k = 1 .. m, summing a(floor(m/k)) */
        for (idx_t k = 1; k <= m; ++k) {
            idx_t idx = m / k;          /* floor((i-1)/k) */
            sum += table[idx];          /* value already computed */
        }
        table[i] = sum;
    }
    max_computed = n;

    return table[n];
}
