/*
 * A037884  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*7^i} is the base 7
 *          representation of n and e(i) are the digits d(i) in nondecreasing
 *          order.
 * https://oeis.org/A037884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * Σ|d(i) – e(i)| where * d(i) are the base‑7 digits
 *            of n (least significant first) * and e(i) are those digits
 *            sorted in nondecreasing order
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* an undefined n is treated as 0 */
    if (n == 0)
        return (seq_t)0;

    /* Base‑7 digits of n, and a copy for sorting. */
    uint8_t d[25];
    uint8_t e[25];
    idx_t x = n;
    size_t len = 0;

    while (x > 0) {
        d[len] = (uint8_t)(x % 7);
        e[len] = d[len];
        x /= 7;
        ++len;
    }

    /* Simple insertion sort of e (len ≤ 23). */
    for (size_t i = 1; i < len; ++i) {
        uint8_t key = e[i];
        size_t j = i;
        while (j > 0 && e[j-1] > key) {
            e[j] = e[j-1];
            --j;
        }
        e[j] = key;
    }

    /* Sum of absolute differences. */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t diff = (d[i] > e[i]) ? (d[i] - e[i]) : (e[i] - d[i]);
        sum += (seq_t)diff;
    }

    /* a(n) is half the sum. */
    return sum / 2;
}
