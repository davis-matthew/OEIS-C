/*
 * A130720  Summation of a sequence of sequential numbers containing the first
 *          term as a substring.
 * https://oeis.org/A130720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, look
 * Formula:   a(n) is the smallest integer N>n that equals n + (n+1) + ... + k
 *            * for some k≥n+1 and whose decimal representation contains the
 *            decimal * representation of n as a substring
 * Range:     u8 n <= 14, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9921 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <string.h>
#include <stddef.h>
#include <stdbool.h>

/* convert an idx_t value to a decimal string.
   buf must be large enough (at least 25 bytes for 128‑bit values). */
static void to_str_idx(idx_t v, char *buf, size_t *len)
{
    if (v == 0) { buf[0] = '0'; *len = 1; return; }
    size_t pos = 0;
    while (v > 0) { unsigned d = (unsigned)(v % 10); buf[pos++] = '0' + d; v /= 10; }
    for (size_t i = 0; i < pos / 2; ++i) { char t = buf[i]; buf[i] = buf[pos-1-i]; buf[pos-1-i] = t; }
    *len = pos;
}

/* convert a seq_t value to a decimal string.
   buf must be large enough (use >2000 bytes for 128‑bit values). */
static void to_str_seq(seq_t v, char *buf, size_t *len)
{
    if (v == 0) { buf[0] = '0'; *len = 1; return; }
    size_t pos = 0;
    while (v > 0) { unsigned d = (unsigned)(v % 10); buf[pos++] = '0' + d; v /= 10; }
    for (size_t i = 0; i < pos / 2; ++i) { char t = buf[i]; buf[i] = buf[pos-1-i]; buf[pos-1-i] = t; }
    *len = pos;
}

/* simple substring test on two character buffers. */
static bool contains(const char *s, size_t slen, const char *pat, size_t plen)
{
    if (plen == 0 || plen > slen) return false;
    for (size_t i = 0; i + plen <= slen; ++i)
        if (memcmp(s + i, pat, plen) == 0) return true;
    return false;
}

seq_t a(idx_t n)
{
    /* maximum decimal length needed: idx_t <= 10^20, seq_t <= ~10^2000 */
    const size_t BUF = 2048;
    char pat[BUF];
    size_t pat_len;
    to_str_idx(n, pat, &pat_len);

    seq_t sum = (seq_t)n;
    seq_t next = (seq_t)(n + 1);
    char sumbuf[BUF];
    size_t sumlen;

    for (;;) {
        /* first test after at least one addition */
        if (sum != (seq_t)n && contains(sumbuf, sumlen, pat, pat_len))
            break;
        sum += next;
        ++next;
        to_str_seq(sum, sumbuf, &sumlen);
    }
    return sum;
}
