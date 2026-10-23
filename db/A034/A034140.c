/*
 * A034140  Number of partitions of n into distinct parts from [1, 10].
 * https://oeis.org/A034140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,…,10} with sum equal to n
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The maximum possible sum is 1+2+…+10 = 55, so a(n)=0 for n>55. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t table[56];      /* table[s] = number of subsets summing to s */
    static bool init = false;

    if (!init) {
        for (idx_t s = 0; s <= 55; ++s)
            table[s] = 0;
        table[0] = 1;           /* empty subset */
        for (idx_t part = 1; part <= 10; ++part) {
            for (idx_t s = 55; s >= part; --s) {
                table[s] += table[s - part];
            }
        }
        init = true;
    }

    if (n > 55) return 0;
    return table[n];
}
