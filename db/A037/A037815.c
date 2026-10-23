/*
 * A037815  Number of i such that d(i) <= d(i-1), where Sum_{i=0..m} d(i)*8^i
 *          is the base-8 representation of n.
 * https://oeis.org/A037815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i≥1 such that digit_i ≤ digit_{i‑1} in
 *            the base‑8 expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Count non‑negative first differences of base‑8 digits. */
    seq_t count = 0;
    if (n == 0)   /* Undefined for n<1, but guard anyway. */
        return count;

    idx_t tmp = n;
    int prev = tmp % 8;      /* least significant digit */
    tmp /= 8;

    while (tmp > 0) {
        int digit = tmp % 8;
        if (digit <= prev)
            ++count;
        prev = digit;
        tmp /= 8;
    }
    return count;
}
