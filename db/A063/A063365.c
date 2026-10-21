/*
 * A063365  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          92 ).
 * https://oeis.org/A063365
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-4) + a(n-6) – a(n-10) for n > 13 with initial values
 *            a(2) = –1 a(3) = 132 a(4) = 286 a(5) = 440 a(6) = 592 a(7) = 746
 *            a(8) = 898 a(9) = 1056 a(10) = 1204 a(11) = 1362 a(12) = 1510
 *            a(13) = 1670
 * Range:     i8 n <= 2, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static table holds computed values up to the largest n requested.
       The array is large enough for the required range (n up to ~10000). */
    static seq_t table[20001] = {0};
    static idx_t cur_max = 1;      /* highest index whose value is known */

    if (n < 2)            /* the behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* Make sure the table contains a(n) */
    if (n > cur_max) {
        /* Pre‑computed initial values for 2 ≤ i ≤ 13 */
        static const seq_t init_vals[12] = {
            -1, 132, 286, 440, 592, 746,
            898, 1056, 1204, 1362, 1510, 1670
        };

        for (idx_t i = cur_max + 1; i <= n; ++i) {
            if (i <= 13)
                table[i] = init_vals[i - 2];
            else
                table[i] = table[i - 4] + table[i - 6] - table[i - 10];
        }
        cur_max = n;
    }

    return table[n];
}
