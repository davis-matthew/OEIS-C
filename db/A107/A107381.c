/*
 * A107381  a(n) = 4*a(n-1)-3*a(n-2) -3*a(n-3) +2*a(n-4) + a(n-5), n>7.
 * https://oeis.org/A107381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)-3*a(n-2)-3*a(n-3)+2*a(n-4)+a(n-5), * initial
 *            values: a(0)=4, a(1)=3, a(2)=9, a(3)=17, a(4)=40, * a(5)=92,
 *            a(6)=215, a(7)=506 * (defined for n≥0; the recurrence uses the
 *            previous 5 terms)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table of computed terms */
    static seq_t *table = NULL;
    static idx_t size = 0;    /* number of computed values (table[0..size-1]) */

    /* ensure table is allocated with at least 8 entries (initial values) */
    if (size < 8) {
        idx_t need = 8;
        if (n + 1 > need)
            need = n + 1;                          /* at least up to n if n<7 */
        table = (seq_t *)realloc(table, need * sizeof(seq_t));
        table[0] = (seq_t)4;
        table[1] = (seq_t)3;
        table[2] = (seq_t)9;
        table[3] = (seq_t)17;
        table[4] = (seq_t)40;
        table[5] = (seq_t)92;
        table[6] = (seq_t)215;
        table[7] = (seq_t)506;
        size   = need;
    }

    /* grow table to hold up to n */
    if (n + 1 > size) {
        idx_t newsize = n + 1;
        table = (seq_t *)realloc(table, newsize * sizeof(seq_t));

        /* compute missing terms */
        for (idx_t i = size; i <= n; ++i) {
            seq_t a1 = table[i-1];
            seq_t a2 = table[i-2];
            seq_t a3 = table[i-3];
            seq_t a4 = table[i-4];
            seq_t a5 = table[i-5];

            /* recurrence: 4*a1 - 3*a2 - 3*a3 + 2*a4 + a5 */
            seq_t val = 4 * a1;      /* integer arithmetic; safe up to the 64‑bit limit */
            val -= 3 * a2;
            val -= 3 * a3;
            val += 2 * a4;
            val += a5;

            table[i] = val;
        }
        size = newsize;
    }

    return table[n];
}
