/*
 * A021391  Decimal expansion of 1/387.
 * https://oeis.org/A021391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the nth decimal digit of 1/387 (a(0) is the first digit)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* static cache of digits computed so far */
    static seq_t *digits = NULL;
    static idx_t digits_len = 0;  /* number of digits stored so far */
    static size_t digits_cap = 0; /* capacity of the array */
    static uint64_t rem = 1;      /* remainder for long division; 0 ≤ rem < 387 */

    /* extend cache if necessary */
    if (digits_len <= n) {
        idx_t new_len = n + 1;
        if ((size_t)new_len > digits_cap) {
            /* grow array to at least new_len elements */
            grow_seq_array(&digits, &digits_cap, (size_t)new_len);
        }

        for (idx_t i = digits_len; i < new_len; ++i) {
            rem *= 10ull;                      /* bring down a zero */
            seq_t d = (seq_t)(rem / 387ull);    /* integer part */
            rem = rem % 387ull;                /* update remainder */
            digits[i] = d;
        }
        digits_len = new_len;
    }

    return digits[n];
}
