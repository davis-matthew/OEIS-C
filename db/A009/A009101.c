/*
 * A009101  Fixed point when iterating the function f on n, where f(x) = x +
 *          product of digits of x.
 * https://oeis.org/A009101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 9819, u32 n <= 9819, u64 n <= 9819,
 *            u128 n <= 9819, big any n
 * Helpers:   digit_frequencies, digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the first number reached by iterating
   f(x) = x + product_of_digits(x) that contains a zero digit.
   For n = 0, a(0) = 0. */

/* The following helpers are provided by the test harness.
   digit_product computes the product of the decimal digits of x.
   digit_frequencies fills an array with the counts of digits 0–9. */
#include "../utils/digit_product.h"
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    unsigned int freq[10];
    /* Check if the starting number already contains a zero. */
    digit_frequencies(x, freq);
    if (freq[0]) return x;

    /* Iterate f until a zero digit appears. */
    while (1)
    {
        seq_t prod = digit_product(x);
        x = x + prod;
        digit_frequencies(x, freq);
        if (freq[0]) return x;
    }
}
