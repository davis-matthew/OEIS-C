/*
 * A147567  a(n) = a(n-4) - a(n-8).
 * https://oeis.org/A147567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-4) - a(n-8); Initial values: a(0)=2, a(1)=1, a(2)=3,
 *            a(3)=4, a(4)=1, a(5)=2, a(6)=-1, a(7)=3. The sequence has period
 *            24
 * Range:     i8 n <= 5000, i16 n <= 5000, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache for the 24‑term period. */
    static seq_t table[24];
    static bool init = false;

    if (!init) {
        /* Set the first 8 values explicitly. */
        table[0] = (seq_t)2;
        table[1] = (seq_t)1;
        table[2] = (seq_t)3;
        table[3] = (seq_t)4;
        table[4] = (seq_t)1;
        table[5] = (seq_t)2;
        table[6] = (seq_t)-1;
        table[7] = (seq_t)3;

        /* Fill the remaining 16 values using the recurrence. */
        for (int i = 8; i < 24; ++i) {
            table[i] = table[i - 4] - table[i - 8];
        }
        init = true;
    }

    return table[(size_t)(n % 24)];
}
