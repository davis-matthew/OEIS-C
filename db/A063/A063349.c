/*
 * A063349  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          76 ).
 * https://oeis.org/A063349
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) - a(n-10) for n > 13 with offset 2: a(2)
 *            = -1, a(3) = 87, a(4) = 192, a(5) = 297, a(6) = 400, a(7) = 505,
 *            a(8) = 608, a(9) = 717, a(10) = 816, a(11) = 925, a(12) = 1024,
 *            a(13) = 1135
 * Range:     i8 n <= 3, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* values below the offset are undefined */

    static seq_t *table = NULL;
    static idx_t table_size = 0;

    if (table_size <= n) {
        /* Expand table to accommodate indices up to n */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(table, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        /* Initialize new slots to 0 */
        if (table_size < new_size)
            for (idx_t i = table_size; i <= n; ++i) tmp[i] = 0;
        table = tmp;
        table_size = (idx_t)new_size;

        /* Initialize base values (if not already set) */
        if (n >= 2) table[2] = -1;
        if (n >= 3) table[3] = 87;
        if (n >= 4) table[4] = 192;
        if (n >= 5) table[5] = 297;
        if (n >= 6) table[6] = 400;
        if (n >= 7) table[7] = 505;
        if (n >= 8) table[8] = 608;
        if (n >= 9) table[9] = 717;
        if (n >=10) table[10] = 816;
        if (n >=11) table[11] = 925;
        if (n >=12) table[12] = 1024;
        if (n >=13) table[13] = 1135;
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = 14; i <= n; ++i) {
        if (table[i] == 0) {
            table[i] = table[i-4] + table[i-6] - table[i-10];
        }
    }

    return table[n];
}
