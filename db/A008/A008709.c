/*
 * A008709  12 in base 12-n.
 * https://oeis.org/A008709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = decimal value of the representation of 12 in base (12-n)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* Helper: number of decimal digits of a non‑negative integer (seq_t compatible). */
static uint32_t num_dec_digits(uint64_t v)
{
    uint32_t d = 0;
    do {
        ++d;
        v /= 10;
    } while (v);
    return d;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    /* Compute the base 12‑n as a signed 64‑bit value.  */
    int64_t base = (int64_t)12 - (int64_t)n;

    /* Base 0 or -1 is not a valid radix for representation.  We
       simply return the ordinary decimal number 12.  */
    if (base == 0 || base == -1) {
        return seq_from_str("12");
    }

    /* Base 1 (unary) is handled specially: 12 in base 1 is twelve 1's. */
    if (base == 1) {
        return seq_from_str("111111111111");
    }

    /* Work with the signed base and the absolute value for remainder calculations. */
    int64_t abs_base = (base > 0) ? base : -base;

    /* Convert the integer 12 into the given base, collecting digits LSB first. */
    int64_t N = 12;
    /* The number of digits will never exceed 64 for the ranges of interest. */
    int64_t digits[64];
    int dcnt = 0;

    while (N != 0) {
        int64_t r = N % abs_base;
        if (r < 0) r += abs_base;  /* ensure a non‑negative remainder < |base|   */
        digits[dcnt++] = r;

        /* Update N for the next step.  Because base can be negative,
           N may become negative, so we work with signed arithmetic.   */
        N = (N - r) / base;
    }

    /* Build the decimal string that results from concatenating the digits.
       The buffer is sufficiently large for the expected size. */
    char numbuf[4096];
    size_t pos = 0;

    for (int i = dcnt - 1; i >= 0; --i) {
        int64_t d = digits[i];
        /* Convert the digit to decimal string. */
        /* snprintf is safe here; the buffer is large enough. */
        int len = snprintf(numbuf + pos, sizeof(numbuf) - pos, "%lld", (long long)d);
        pos += (size_t)len;
    }
    numbuf[pos] = '\0';

    /* Interpret the concatenated decimal string as an arbitrary‑precision integer. */
    return seq_from_str(numbuf);
}
