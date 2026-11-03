/*
 * A118389  Least triangular number T(k) that contains the consecutive digits
 *          of n, where T(k) = k*(k+1)/2.
 * https://oeis.org/A118389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest triangular number T(k) that contains the decimal
 *            representation of n as a consecutive substring. Triangular
 *            numbers are defined by T(k) = k*(k+1)/2 (k >= 0)
 * Range:     u8 n <= 10, u16 n <= 108, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9581 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Convert an integer of type idx_t or seq_t to a decimal string.
   The digits are stored in buf[0..len-1].  The function returns
   the number of digits written.  The buffer must be large enough
   to hold the result (max 64 for any 128‑bit value). */
static size_t to_decimal(seq_t value, char *buf)
{
    if (value == 0) {
        buf[0] = '0';
        return 1;
    }
    seq_t tmp = value;
    size_t len = 0;
    while (tmp != 0) {
        int digit = (int)(tmp % 10);
        buf[len++] = '0' + digit;
        tmp /= 10;
    }
    /* reverse the string in place */
    for (size_t i = 0; i < len / 2; ++i) {
        char c = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = c;
    }
    return len;
}

/* Simple substring search: check whether s[0..nlen-1] occurs in
   t[0..tlen-1].  Returns 1 if found, 0 otherwise. */
static int contains_substring(const char *tbuf, size_t tlen,
                               const char *sbuf, size_t slen)
{
    if (slen == 0 || slen > tlen) return 0;
    for (size_t i = 0; i + slen <= tlen; ++i) {
        size_t j = 0;
        while (j < slen && tbuf[i + j] == sbuf[j]) ++j;
        if (j == slen) return 1;
    }
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Convert n to decimal string once */
    char nbuf[32];
    size_t nlen = to_decimal((seq_t)n, nbuf);

    /* Scan triangular numbers until one contains n */
    for (idx_t k = 0;; ++k) {
        seq_t t = (seq_t)k * ((seq_t)k + 1) / 2;

        /* Convert t to decimal and search */
        char tbuf[64];
        size_t tlen = to_decimal(t, tbuf);

        if (contains_substring(tbuf, tlen, nbuf, nlen))
            return t;
    }
}
