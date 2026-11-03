/*
 * A061818  Multiples of 2 containing only digits 0,1,2.
 * https://oeis.org/A061818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   this sequence lists all decimal numbers composed of the digits
 *            0,1,2 that are even (last digit 0 or 2); the first term is 0
 *            (offset 0). The nth term (0‑based) is given by a mixed radix
 *            construction: first digit 1 or 2, middle digits 0–2, last digit
 *            0 or 2
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: build the number of given length 'len' (len>=2) from its
   0‑based offset within that block. */
static seq_t build_number(idx_t offset, uint8_t len)
{
    if (len == 1) {
        /* len == 1 occurs only for the single number 2 (zero handled
           separately). */
        return (seq_t)2;
    }

    /* Compute the array of radices: [2, 3, 3, …, 3, 2] */
    unsigned rad[32];   /* len will never exceed 32 for realistic n. */
    rad[0] = 2;
    for (uint8_t i = 1; i <= len - 2; ++i) rad[i] = 3;
    rad[len - 1] = 2;

    /* Suffix products: suffix[i] = product of rad[i] .. rad[len-1] */
    idx_t suffix[33];
    suffix[len] = 1;
    for (int i = len - 1; i >= 0; --i) {
        suffix[i] = rad[i] * suffix[i + 1];
    }

    /* Generate the digits from most significant to least. */
    seq_t result = 0;
    idx_t x = offset;
    for (uint8_t i = 0; i < len; ++i) {
        idx_t digit_idx = x / suffix[i + 1];
        x %= suffix[i + 1];

        unsigned digit;
        if (i == 0) {
            /* First digit: 1 or 2. */
            digit = 1 + (unsigned)digit_idx; /* digit_idx = 0 → 1, 1 → 2 */
        } else if (i == len - 1) {
            /* Last digit: 0 or 2. */
            digit = 2 * (unsigned)digit_idx; /* 0 → 0, 1 → 2 */
        } else {
            /* Middle digits: 0,1,2. */
            digit = (unsigned)digit_idx;
        }
        result = result * 10 + digit;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */

    /* Handle numbers > 0.  Subtract the zero term from the index. */
    idx_t remaining = n - 1;              /* zero‑based among positive terms */

    /* Numbers with a single non‑zero digit are just "2". */
    if (remaining < 1) return (seq_t)2;    /* a(1) = 2 */
    remaining -= 1;

    /* Iterate over increasing digit lengths. */
    idx_t pow3 = 1;                       /* 3^(len-2);  for len=2 it's 3^0=1. */
    for (uint8_t len = 2; ; ++len) {
        idx_t count_len = 4 * pow3;       /* count of len‑digit even numbers */

        if (remaining < count_len) {
            /* The desired number lies in this block. */
            return build_number(remaining, len);
        }

        remaining -= count_len;           /* skip this block */
        pow3 *= 3;                        /* prepare for next length */
    }
}
