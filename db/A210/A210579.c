/*
 * A210579  a(1)=2, a(n+1)=a(n)*(n-th digit of the sequence or 10 in case of
 *          digit '0').
 * https://oeis.org/A210579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 2, a(n+1) = a(n) * d, where d is the n-th digit of the *
 *            concatenated decimal representation of the sequence, and d = 10
 *            if the * digit is 0. Saves previously computed terms and digits
 *            to allow * out‑of‑order queries
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   digit_len, ensure_capacity, seq_alloc, seq_mul, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/seq_alloc.h"
#include "../utils/ensure_capacity.h"
#include "../utils/seq_to_str.h"
#include "../utils/seq_mul.h"
#include "../utils/digit_len.h"

/* Static buffers for memoisation */
static seq_t *values = NULL;
static size_t values_cap = 0;
static size_t values_len = 0;

static uint8_t *digits = NULL;
static size_t digits_cap = 0;
static size_t digits_len = 0;

/* Append the decimal digits of v to the global digit buffer. */
static void append_digits(seq_t v)
{
    size_t dlen = digit_len(v);
    char *buf = (char *)malloc(dlen + 1);
    if (!buf) return;   /* allocation failure; silently ignore – caller will fail */
    seq_to_str(v, buf, &dlen);

    ensure_capacity(digits_len + dlen, &digits_cap, (void **)&digits, sizeof(uint8_t));
    for (size_t i = 0; i < dlen; ++i) {
        digits[digits_len + i] = (uint8_t)(buf[i] - '0');
    }
    digits_len += dlen;
    free(buf);
}

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for the used ranges */

    /* Initialise first term once. */
    if (values_len == 0) {
        seq_t first = (seq_t)2;
        ensure_capacity(1, &values_cap, (void **)&values, sizeof(seq_t));
        values[0] = first;
        values_len = 1;

        /* Initialise digit buffer. */
        digits_len = 0;
        digits_cap = 0;
        append_digits(first);
    }

    /* Extend precomputation to the required index. */
    while (values_len < (size_t)n) {
        seq_t prev = values[values_len - 1];
        uint8_t dig = digits[values_len - 1];
        seq_t mult = (dig == 0 ? (seq_t)10 : (seq_t)dig);
        seq_t next = seq_mul(prev, mult);

        ensure_capacity(values_len + 1, &values_cap, (void **)&values, sizeof(seq_t));
        values[values_len] = next;
        values_len++;

        append_digits(next);
    }

    return values[n - 1];
}
