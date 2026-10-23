/*
 * A030604  Write the Fibonacci numbers in base 6 and juxtapose.
 * https://oeis.org/A030604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the nth digit (0‑5) obtained by writing the Fibonacci
 *            numbers in base 6 and concatenating them
 * Range:     u8 n <= 26, u16 n <= 83, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static void ensure_digits_capacity(size_t needed, seq_t **buf,
                                   size_t *len, size_t *cap)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap > 0) ? (*cap) * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*buf, new_cap * sizeof(seq_t));
    *buf = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *digits          = NULL;
    static size_t digits_len      = 0;
    static size_t digits_cap      = 0;
    static seq_t f_prev = 0, f_curr = 1;
    static bool initialized = false;

    if (!initialized) {
        digits_cap = 16;
        digits = (seq_t *)malloc(digits_cap * sizeof(seq_t));
        if (!digits) return 0; /* allocation failed */
        initialized = true;
    }

    /* Generate Fibonacci digits until we have covered index n */
    while (digits_len <= (size_t)n) {
        seq_t val = f_prev;

        /* Convert val to base‑6 digits */
        if (val == 0) {
            ensure_digits_capacity(digits_len + 1, &digits,
                                   &digits_len, &digits_cap);
            digits[digits_len++] = 0;
        } else {
            /* Temporary storage for the digits (reversed order) */
            seq_t tmp[256];
            size_t tmp_cnt = 0;
            seq_t temp = val;
            while (temp > 0) {
                tmp[tmp_cnt++] = temp % 6;
                temp = temp / 6;
            }
            /* Append digits in correct order */
            ensure_digits_capacity(digits_len + tmp_cnt, &digits,
                                   &digits_len, &digits_cap);
            for (size_t i = 0; i < tmp_cnt; ++i)
                digits[digits_len++] = tmp[tmp_cnt - 1 - i];
        }

        /* Advance to the next Fibonacci number */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }

    return digits[(size_t)n];
}
