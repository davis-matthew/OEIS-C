/*
 * A111704  a(n) = concatenation of (n times each digit of n).
 * https://oeis.org/A111704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat ( n*d for each decimal digit d of n )
 * Range:     u8 n <= 10, u16 n <= 33, u32 n <= 166, u64 n <= 4632,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Convert n to a decimal string to traverse its digits in order. */
    char buf[32];
    int len = snprintf(buf, sizeof buf, "%llu", (unsigned long long)n);

    seq_t result = 0;
    for (int i = 0; i < len; ++i) {
        unsigned int digit = (unsigned int)(buf[i] - '0');
        seq_t prod = (seq_t)n * digit;

        idx_t plen = prod == 0 ? 1 : digit_len(prod);   /* treat 0 as length 1 */
        seq_t power = pow10(plen);

        result = result * power + prod;
    }
    return result;
}
