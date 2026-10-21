/*
 * A217179  a(n) is the number of digits in the decimal representation of the
 *          smallest power of n that contains four consecutive identical
 *          digits.
 * https://oeis.org/A217179
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4714, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_four_run
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by brute‑force search of the smallest power n^k that contains
   four consecutive identical decimal digits.  The function returns the number
   of digits of that power.  It works with any seq_t (8/16/32/64‑bit integers
   or an arbitrary‑precision integer type).  The algorithm uses a decimal
   string representation to avoid overflow on the 64‑bit builds.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/has_four_run.h"

/* Multiply the decimal number represented by digits array (little endian)
   by a small integer m (2 ≤ m ≤ 10000).  The array size after multiplication
   is stored back in *len.  */
static void mul_small(uint8_t *d, size_t *len, uint32_t m)
{
    uint32_t carry = 0;
    for (size_t i = 0; i < *len; ++i) {
        uint32_t prod = d[i] * m + carry;
        d[i] = prod % 10;
        carry = prod / 10;
    }
    while (carry) {
        d[*len] = carry % 10;
        carry /= 10;
        ++(*len);
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;

    /* Buffer to hold the decimal digits of the current power.  */
    /* 2000 digits is more than enough for the expected range.  */
    uint8_t digits[2000];
    size_t len = 0;

    /* Initialise digits with the value of n.  */
    uint64_t tmp = (uint64_t)n;
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    /* Special case if n itself already contains a run of four equal digits.
       This can happen for n like 4444.  */
    if (has_four_run(digits, len)) return (seq_t)len;

    /* Iteratively multiply by n until we find a run of four equal digits.  */
    for (;;) {
        mul_small(digits, &len, (uint32_t)n);
        if (has_four_run(digits, len)) return (seq_t)len;
    }
}
