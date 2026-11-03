/*
 * A107975  First differences give the same sequence written as a string of
 *          individual digits.
 * https://oeis.org/A107975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A107975, defined by “first differences give the same sequence written
   as a string of individual digits.”  We build the sequence by storing the
   decimal digits of each term; the next term is obtained by adding the next
   unused digit to the previous term. */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* enlarge an array of unsigned char elements if the minimum capacity is not met */
static void ensure_char_cap(unsigned char **arr, size_t *cap, size_t min_cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0) ? 64 : *cap;
        while (new_cap < min_cap) new_cap <<= 1;
        *arr = (unsigned char *)realloc(*arr, new_cap);
        *cap = new_cap;
    }
}

/* append the decimal digits of a seq_t value to the digit stream.  The
   digits are stored as the integer 0–9.  The most significant digit is
   appended first. */
static void append_value_digits(seq_t val,
                                 unsigned char **digits,
                                 size_t *cap,
                                 size_t *len)
{
    unsigned char tmp[40];                /* maximum digits for 128‑bit value */
    int pos = 0;
    seq_t tmp_val = val;

    if (tmp_val == (seq_t)0) {
        tmp[pos++] = 0;
    } else {
        while (tmp_val != (seq_t)0) {
            tmp[pos++] = (unsigned char)(tmp_val % (seq_t)10);
            tmp_val /= (seq_t)10;
        }
    }

    /* append the digits in correct order (most significant first) */
    for (int i = pos - 1; i >= 0; --i) {
        if (*len >= *cap)
            ensure_char_cap(digits, cap, *len + 1);
        (*digits)[(*len)++] = tmp[i];
    }
}

/* enlarge an array of seq_t elements if the minimum capacity is not met */
static void ensure_seq_cap(seq_t **arr, size_t *cap, size_t min_cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap == 0) ? 16 : *cap;
        while (new_cap < min_cap) new_cap <<= 1;
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
}

/* --------------------------------------------------------------- */
/*  Main function required by the specification                   */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static storage for the sequence and the digit stream */
    static seq_t *vals = NULL;
    static size_t val_cap = 0;
    static idx_t val_len = 0;          /* number of computed sequence terms */

    static unsigned char *digits = NULL;
    static size_t digit_cap = 0;
    static size_t digit_len = 0;       /* total number of digits in the stream */
    static size_t digit_head = 0;      /* index of next digit to use */

    /* first call: initialise with a(1) = 5 */
    if (val_len == 0) {
        ensure_seq_cap(&vals, &val_cap, 16);
        vals[0] = (seq_t)5;
        val_len = 1;

        ensure_char_cap(&digits, &digit_cap, 64);
        append_value_digits((seq_t)5, &digits, &digit_cap, &digit_len);
    }

    /* return if already computed */
    if (n <= val_len) return vals[n - 1];

    /* compute subsequent terms until we reach n */
    while (val_len < n) {
        if (digit_head >= digit_len) {
            /* the construction has failed; return 0 as a safeguard */
            return (seq_t)0;
        }
        unsigned char diff = digits[digit_head++];
        seq_t next_val = vals[val_len - 1] + (seq_t)diff;

        ensure_seq_cap(&vals, &val_cap, val_len + 1);
        vals[val_len] = next_val;
        val_len++;

        append_value_digits(next_val, &digits, &digit_cap, &digit_len);
    }

    return vals[n - 1];
}
