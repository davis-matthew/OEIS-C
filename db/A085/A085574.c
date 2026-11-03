/*
 * A085574  Least k such that n*(n+1)*(n+2) + k is palindromic.
 * https://oeis.org/A085574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k≥0 such that n*(n+1)*(n+2)+k is a decimal
 *            palindrome
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Build a seq_t from an array of decimal digits (most‑significant first). */
static seq_t digits_to_seq(const uint8_t *buf, size_t len)
{
    seq_t val = 0;
    for (size_t i = 0; i < len; ++i) {
        val = val * 10 + buf[i];
    }
    return val;
}

/* Add 1 to the middle part of a decimal number represented by its digits
   (buf[0..len-1]), where the middle for even len is the left‑hand part
   of the centre.  The buffer is modified in place and the new length is
   returned via *len.  If an overflow occurs (e.g. 999 → 1001) the
   buffer is adjusted accordingly. */
static void inc_middle(uint8_t *buf, size_t *len)
{
    size_t i;
    bool odd = (*len % 2 == 1);
    if (odd) {
        i = *len / 2;   /* middle digit */
        while (i < *len) {  /* propagate carry leftwards */
            uint8_t d = buf[i] + 1;
            if (d == 10) {
                buf[i] = 0;
                i--;  /* carry to the left */
                if (i == (size_t)-1) break; /* will handle overflow later */
            } else {
                buf[i] = d;
                return;
            }
        }
    } else {
        i = *len / 2 - 1;  /* left side of middle for even length */
        while (i < *len) {  /* propagate carry leftwards */
            uint8_t d = buf[i] + 1;
            if (d == 10) {
                buf[i] = 0;
                i--;
                if (i == (size_t)-1) break;
            } else {
                buf[i] = d;
                return;
            }
        }
    }
    /* overflow: 999...9 → 1000...001 */
    *len = *len + 1;
    for (size_t j = *len - 1; j > 0; --j) {
        buf[j] = 0;
    }
    buf[0]   = 1;
    buf[*len - 1] = 1;
}

/* Convert a seq_t number into its decimal digits (most‑significant digit first)
   and return the length.  The buffer must be at least 128 bytes (enough for
   seq_t values up to 128 decimal digits). */
static void seq_to_digits(seq_t val, uint8_t *buf, size_t *len)
{
    if (val == 0) {
        buf[0] = 0;
        *len   = 1;
        return;
    }
    size_t i = 0;
    while (val != 0) {
        uint8_t d = (uint8_t)(val % 10);
        buf[i++] = d;
        val /= 10;
    }
    /* reverse to most significant first */
    for (size_t j = 0; j < i / 2; ++j) {
        uint8_t t = buf[j];
        buf[j]    = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    *len = i;
}

/* Construct the smallest palindrome ≥ n.  n is given as a seq_t.
   The function works directly on decimal digits and returns that
   palindrome as a seq_t. */
static seq_t next_pal(seq_t n)
{
    uint8_t dig[128];
    size_t len;
    seq_to_digits(n, dig, &len);

    /* create palindrome by mirroring the left half onto the right */
    uint8_t paldig[128];
    for (size_t i = 0; i < len; ++i) paldig[i] = dig[i];
    for (size_t i = 0; i < len / 2; ++i) {
        paldig[len - 1 - i] = paldig[i];
    }
    seq_t pal = digits_to_seq(paldig, len);
    if (pal >= n) return pal;

    /* not enough, increment the middle part and rebuild */
    inc_middle(paldig, &len);
    /* after increment the left part may have changed, mirror again */
    for (size_t i = 0; i < len / 2; ++i) {
        paldig[len - 1 - i] = paldig[i];
    }
    return digits_to_seq(paldig, len);
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t base = sn * (sn + 1) * (sn + 2);

    /* if base itself is palindromic then k=0 */
    seq_t pal = next_pal(base);
    if (pal == base) return 0;
    return pal - base;
}
