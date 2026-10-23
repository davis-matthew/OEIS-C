/*
 * A037832  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*9^i
 *          is the base-9 representation of n.
 * https://oeis.org/A037832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i ≥ 1 for which the i‑th base‑9 digit
 *            is greater than or equal to the (i‑1)‑th digit of n, where n =
 *            Σ_{k=0..m} d(k)·9^k and 0 ≤ d(k) ≤ 8
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count qualifying positions. */
    seq_t res = 0;

    /* For n = 0 (outside the defined offset) return 0. */
    if (n == 0)
        return res;

    /* Extract base‑9 digits, least significant first. */
    unsigned char digits[64];   /* 64 > max number of base‑9 digits for a 64‑bit n */
    size_t len = 0;
    idx_t x = n;
    while (x > 0) {
        digits[len++] = (unsigned char)(x % 9);
        x /= 9;
    }

    /* Count i such that digits[i] ≥ digits[i−1] for i ≥ 1. */
    for (size_t i = 1; i < len; ++i)
        if (digits[i] >= digits[i - 1])
            ++res;

    return res;
}
