/*
 * A216556  Concatenate decimal digits of n, each increased by 1.
 * https://oeis.org/A216556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = concatenation of decimal digits of n each increased by 1.
 *            For example, n = 19 yields a(19) = concat(2,10) = 210
 * Range:     u8 n <= 28, u16 n <= 698, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>

seq_t a(idx_t n)
{
    /* Convert the index to its decimal representation. */
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%llu", (unsigned long long)n);

    /* Build the concatenated result digit by digit. */
    seq_t res = 0;
    for (int i = 0; i < len; ++i) {
        int d   = buf[i] - '0';   /* current decimal digit */
        int inc = d + 1;          /* digit increased by 1 */

        if (inc == 10) {          /* only possible for digit 9 */
            res = res * 100 + 10; /* append the two digits "10" */
        } else {
            res = res * 10 + inc; /* append a single digit */
        }
    }
    return res;
}
