/*
 * A096293  Number of iterations of n -> n + (sum of squares of digits of n)
 *          needed for the trajectory of n to join the trajectory of A033936.
 * https://oeis.org/A096293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of iterations of n -> n + sum of squares of
 *            digits(n) until the trajectory joins the trajectory of A033936
 *            (the 1‑chain)
 * Range:     u8 n <= 2, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   digit_len, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/get_digits.h"
#include "../utils/digit_len.h"

/* --- helper: sum of squares of decimal digits for type seq_t ---------------- */

static seq_t digit_square_sum_seq(seq_t x)
{
    /* Get the number of decimal digits of the current value. */
    size_t dlen = (size_t)digit_len(x);

    /* Allocate a buffer for the digits.  256 digits are enough for
       ordinary 64‑bit integers and for the test values used in this
       project.  Dynamically allocate if more are required. */
    uint8_t stackbuf[256];
    uint8_t *digits = stackbuf;

    if (dlen > sizeof(stackbuf))
        digits = (uint8_t *)malloc(dlen);

    size_t len;
    get_digits(x, digits, &len);

    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += (seq_t)digits[i] * (seq_t)digits[i];

    if (digits != stackbuf)
        free(digits);

    return sum;
}

/* --- main sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic domain. */
    seq_t a_val = (seq_t)n;   /* trajectory starting point */
    seq_t b_val = 1;          /* start of the A033936 (1‑chain) */
    seq_t steps = 0;          /* number of iterations applied to a_val only */

    while (a_val != b_val)
    {
        if (a_val < b_val)
        {
            a_val += digit_square_sum_seq(a_val);
            ++steps;
        }
        else
        {
            b_val += digit_square_sum_seq(b_val);
        }
    }
    return steps;
}
