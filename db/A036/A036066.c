/*
 * A036066  The summarize Lucas sequence: summarize the previous two terms,
 *          start with 1, 3.
 * https://oeis.org/A036066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 758 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A036066 – The summarize Lucas sequence
 *  a(0) = 1, a(1) = 3
 *  a(n+1) = A244112(concat(a(n), a(n-1))),  n >= 1
 *  where A244112 summarizes the digits of its argument:
 *  For each digit d that occurs, output the count of d followed by d,
 *  digits considered in decreasing order.
 *  After index 26 the sequence becomes periodic with period 46.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static void add_digit_counts(seq_t val, seq_t freq[10])
{
    while (val > 0) {
        uint8_t d = (uint8_t)(val % 10);
        ++freq[d];
        val /= 10;
    }
}

static void append_count_digit(seq_t *res, seq_t count, uint8_t digit)
{
    // number of decimal digits of count
    idx_t len = 0;
    seq_t tmp = count;
    while (tmp > 0) {
        tmp /= 10;
        ++len;
    }
    *res = (*res) * pow10(len);
    *res = *res + count;
    *res = (*res) * 10 + digit;
}

static seq_t *seq   = NULL;   /* cache of sequence values */
static size_t seq_cap = 0;    /* allocated capacity */
static bool   initialized = false;

static void init_cache(void)
{
    /* allocate space for indices 0..73 (74 values) */
    if (!seq) {
        grow_seq_array(&seq, &seq_cap, 74);
    }
    /* compute terms 0..73 */
    for (idx_t i = 0; i <= 73; ++i) {
        seq_t freq[10];
        for (int d = 0; d < 10; ++d) freq[d] = 0;

        if (i == 0) {
            seq[i] = 1;
            continue;
        }
        if (i == 1) {
            seq[i] = 3;
            continue;
        }
        /* aggregate digit counts from the two previous terms */
        add_digit_counts(seq[i-1], freq);
        add_digit_counts(seq[i-2], freq);

        /* build the next term by appending count–digit pairs
           in decreasing digit order (9 .. 0) */
        seq_t val = 0;
        for (int d = 9; d >= 0; --d) {
            if (freq[d] > 0) {
                append_count_digit(&val, freq[d], (uint8_t)d);
            }
        }
        seq[i] = val;
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_cache();
    }

    /* the sequence is periodic from index 27 onwards, period 46 */
    const idx_t period_start = 27;
    const idx_t period_len   = 46;

    if (n < period_start + period_len) {   /* values before cycle end */
        return seq[n];
    }
    /* map large n into the period */
    idx_t mapped = period_start + (n - period_start) % period_len;
    return seq[mapped];
}
