/*
 * A099842  Expansion of (1-x)/(1 + 6*x - 3*x^2).
 * https://oeis.org/A099842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-7 a(n) = -6*a(n-1) + 3*a(n-2) for n>=2 (recurrence
 *            from G.f. (1-x)/(1+6x-3x^2))
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 23, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* precompute up to 10000 via a small static table */
    static seq_t table[10001];
    static idx_t table_filled = 0;   /* next index to compute; 0 means none yet */

    if (n < 10001) {
        /* initialise table on first use */
        if (table_filled == 0) {
            table[0] = (seq_t)1;
            table[1] = (seq_t)-7;
            table_filled = 2;
        }
        if (n >= table_filled) {
            for (idx_t i = table_filled; i <= n; ++i) {
                table[i] = -(seq_t)6 * table[i - 1] + (seq_t)3 * table[i - 2];
            }
            table_filled = n + 1;
        }
        return table[n];
    }

    /* for n >= 10001 compute iteratively without storage */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-7;
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)-7;
    seq_t ax;
    for (idx_t i = 2; i <= n; ++i) {
        ax = -(seq_t)6 * a1 + (seq_t)3 * a0;
        a0 = a1;
        a1 = ax;
    }
    return a1;
}
