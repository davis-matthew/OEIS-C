/*
 * A083311  Minimal number of edit-steps (insert, delete, substitute) to
 *          obtain in decimal representation the least number greater n that
 *          contains all digits of n (A083310).
 * https://oeis.org/A083311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = LD-10(n, m), where m = A083310(n) is the least integer m
 *            > n that contains all decimal digits of n. LD-10 is the
 *            Levenshtein (edit) distance on decimal strings. This
 *            implementation uses the provided helpers seq_to_str (to obtain
 *            the decimal representation) and levenshtein (to compute the e...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 106 terms through n = 105 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

/* Levenshtein distance between two bit arrays of lengths len1 and len2. */
static int levenshtein(const uint8_t *a, int len1, const uint8_t *b, int len2)
{
    /* Two rows of DP, indices 0..len2.  Size is small (≪128). */
    int prev[128] = {0};
    int curr[128] = {0};

    for (int j = 0; j <= len2; ++j) prev[j] = j;

    for (int i = 1; i <= len1; ++i) {
        curr[0] = i;
        for (int j = 1; j <= len2; ++j) {
            int cost = (a[i-1] == b[j-1]) ? 0 : 1;
            int del = prev[j] + 1;
            int ins = curr[j-1] + 1;
            int sub = prev[j-1] + cost;
            int val = del < ins ? del : ins;
            val = sub < val ? sub : val;
            curr[j] = val;
        }
        for (int j = 0; j <= len2; ++j) prev[j] = curr[j];
    }
    return prev[len2];
}

/* maximum number of decimal digits needed for any seq_t value in the test
   range.  128 is comfortably larger than 39 digits of a 128‑bit integer. */
#define MAX_DIGITS 128

static inline void seq_to_digit_array(seq_t v, uint8_t *out, int *len) {
    /* Convert the sequence value v to its decimal string representation
       and then to an array of digits. */
    char buf[MAX_DIGITS];
    size_t blen = 0;
    seq_to_str(v, buf, &blen);
    for (size_t i = 0; i < blen; ++i)
        out[i] = (uint8_t)(buf[i] - '0');  /* '0' … '9' -> 0…9 */
    *len = (int)blen;
}

seq_t a(idx_t n) {
    uint8_t n_digits[MAX_DIGITS];
    uint8_t m_digits[MAX_DIGITS];
    int n_len, m_len;
    bool n_has[10] = {0};
    bool m_has[10];

    /* Convert n to digit array and record its digit set. */
    seq_to_digit_array((seq_t)n, n_digits, &n_len);
    for (int i = 0; i < n_len; ++i)
        n_has[n_digits[i]] = true;

    /* Search for the least m > n that contains all digits of n. */
    seq_t m = ((seq_t)n) + 1;
    for (;;) {
        seq_to_digit_array(m, m_digits, &m_len);

        /* Build digit set for m. */
        for (int d = 0; d < 10; ++d) m_has[d] = false;
        for (int i = 0; i < m_len; ++i)
            m_has[m_digits[i]] = true;

        /* Check that all digits of n appear in m. */
        bool ok = true;
        for (int d = 0; d < 10; ++d)
            if (n_has[d] && !m_has[d]) { ok = false; break; }

        if (ok) break;
        ++m;
    }

    /* Compute the Levenshtein distance between the decimal strings of n and m. */
    int dist = levenshtein(n_digits, n_len, m_digits, m_len);
    return (seq_t)dist;
}
