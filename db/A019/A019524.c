/*
 * A019524  Duplicate terms of A007908.
 * https://oeis.org/A019524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = concat(1..n) concatenated with itself Initial value: a(1)
 *            = 11 Description: for n ≥ 1, build the decimal string of all
 *            integers from 1 to n, then duplicate it to produce the nth term
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

/* Write the decimal representation of v into buf.
   Returns the number of characters written. */
static size_t write_idx(idx_t v, char *buf)
{
    if (v == 0) {
        buf[0] = '0';
        return 1;
    }
    char tmp[32];
    int pos = 0;
    while (v > 0) {
        tmp[pos++] = '0' + (int)(v % 10);
        v /= 10;
    }
    for (int i = pos - 1; i >= 0; --i) {
        *buf++ = tmp[i];
    }
    return (size_t)pos;
}

/* Count the total number of decimal digits needed to write all
   integers from 1 to n. */
static size_t digit_count_u64(idx_t n)
{
    size_t total = 0;
    idx_t pow10 = 1;
    int len = 1;
    while (pow10 <= n) {
        idx_t end = pow10 * 10 - 1;
        if (end > n) end = n;
        total += (size_t)(end - pow10 + 1) * (size_t)len;
        pow10 *= 10;
        ++len;
    }
    return total;
}

seq_t a(idx_t n)
{
    /* Build the string of numbers 1..n */
    size_t len_part = digit_count_u64(n);
    char *part = (char *)malloc(len_part + 1);
    if (!part) return 0;               /* allocation failure */

    size_t pos = 0;
    for (idx_t i = 1; i <= n; ++i) {
        pos += write_idx(i, part + pos);
    }
    part[pos] = '\0';

    /* Duplicate the part to form the final number */
    size_t len_full = len_part * 2;
    char *full = (char *)malloc(len_full + 1);
    if (!full) {
        free(part);
        return 0;
    }
    memcpy(full, part, len_part);
    memcpy(full + len_part, part, len_part);
    full[len_full] = '\0';

    seq_t result = seq_from_str(full);

    free(part);
    free(full);
    return result;
}
