/*
 * A009441  a(n) is the concatenation of n and 7n.
 * https://oeis.org/A009441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less, easy
 * Formula:   a(n) = concat(n, 7n)
 * Range:     u8 n <= 2, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t seven_n = (seq_t)n * (seq_t)7;
    int k = digit_len(seven_n);
    seq_t pow = pow10((idx_t)k);
    return (seq_t)n * pow + seven_n;
}
