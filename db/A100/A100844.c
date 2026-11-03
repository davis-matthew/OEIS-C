/*
 * A100844  Smallest m greater than n such that m^2 contains n^2 in its
 *          decimal representation.
 * https://oeis.org/A100844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 73 terms through n = 966 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A100844 – Smallest m > n such that m^2 contains n^2 as a decimal substring. */

#include <string.h>   /* for strstr */

/* Convert a seq_t value to a decimal string.  The buffer must be
   large enough to hold the result.  */
static void seq_to_dec_string(seq_t val, char *buf, size_t buflen)
{
    seq_t v = val;
    if (v == 0) {
        if (buflen >= 2) {
            buf[0] = '0';
            buf[1] = '\0';
        }
        return;
    }
    size_t pos = 0;
    /* Extract digits in reverse order. */
    while (v != 0 && pos + 1 < buflen) {
        seq_t d = v % 10;
        v = v / 10;
        buf[pos++] = (char)('0' + (int)d);
    }
    /* Null‑terminate and reverse the string. */
    if (pos >= buflen) pos = buflen - 1;
    buf[pos] = '\0';
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
}

/*  Return a(n).  */
seq_t a(idx_t n)
{
    /* n² */
    seq_t n_sq = (seq_t)n * ((seq_t)n);
    char n_str[64];
    seq_to_dec_string(n_sq, n_str, sizeof(n_str));

    seq_t m = (seq_t)n + 1;
    for (;;) {
        seq_t m_sq = m * m;
        char m_str[64];
        seq_to_dec_string(m_sq, m_str, sizeof(m_str));
        if (strstr(m_str, n_str) != NULL) {
            return m;
        }
        ++m;
        /* The problem guarantees that a(n) ≤ 10·n for n > 0,
           so the loop will terminate quickly. For n = 0
           the sequence value is 10. */
    }
}
