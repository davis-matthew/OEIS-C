/*
 * A037803  Number of i such that d(i) < d(i-1), where Sum_{i=0..m} d(i)*5^i
 *          is the base-5 representation of n.
 * https://oeis.org/A037803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i (1 <= i <= m) such that d(i) <
 *            d(i-1), * where n = Σ_{i=0}^m d(i)*5^i is the base‑5
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t temp = n;
    /* Least significant digit */
    seq_t prev = temp % 5;
    temp /= 5;
    while (temp > 0) {
        seq_t curr = temp % 5;
        if (curr < prev)
            ++count;
        prev = curr;
        temp /= 5;
    }
    return (seq_t)count;
}
