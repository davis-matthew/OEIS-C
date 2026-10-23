/*
 * A021943  Decimal expansion of 1/939.
 * https://oeis.org/A021943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes nth decimal digit of 1/939 (offset 0) */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *digits = NULL;
    static size_t digits_cap = 0;
    static size_t digits_len = 0;
    static seq_t rem = 1;   /* initial remainder = 1/939 */

    if (digits == NULL) {
        digits_cap = 128;
        digits = (seq_t *)malloc(digits_cap * sizeof(seq_t));
        if (!digits) abort();
    }

    if (digits_len <= (size_t)n) {
        size_t target = (size_t)n + 1;
        if (digits_cap < target) {
            size_t new_cap = digits_cap ? digits_cap * 2 : 128;
            while (new_cap < target) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(digits, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            digits = tmp;
            digits_cap = new_cap;
        }
        for (size_t i = digits_len; i < target; ++i) {
            seq_t prod   = rem * 10;
            seq_t digit  = prod / 939;
            digits[i]    = digit;
            rem          = prod - digit * 939;
        }
        digits_len = target;
    }

    return digits[(size_t)n];
}
