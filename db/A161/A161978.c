/*
 * A161978  a(n) = the smallest positive integer that when written in binary
 *          contains both binary n and {the string formed from the reversal of
 *          the binary digits of n, with leading 0's removed} as (possibly
 *          overlapping) substrings.
 * https://oeis.org/A161978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 642, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 83 terms through n = 4910 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A161978 – smallest positive integer whose binary form
   contains both the binary representation of n and the binary
   representation of the reversal of n with leading zeros removed.
   The algorithm builds the two patterns and returns the shorter
   string that contains both as substrings. */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Convert binary string s (MSB first) to a seq_t value. */
static seq_t bin_to_seq(const char *s)
{
    seq_t v = 0;
    for (const char *p = s; *p; ++p)
        v = (v << 1) | (seq_t)(*p - '0');
    return v;
}

/* Return the length of the longest suffix of a (length la) which
   matches a prefix of b (length lb). */
static size_t overlap_len(const char *a, size_t la, const char *b, size_t lb)
{
    size_t m = la < lb ? la : lb;
    for (size_t k = m; k > 0; --k) {
        if (memcmp(a + la - k, b, k) == 0)
            return k;
    }
    return 0;
}

/* Convert n to a binary string in buf (MSB first, '\0' terminated).
   Returns the length of the string (excluding the terminator). */
static size_t to_bin_string(idx_t n, char *buf)
{
    char tmp[128];
    size_t pos = 0;
    do {
        tmp[pos++] = (n & 1) ? '1' : '0';
        n >>= 1;
    } while (n != 0);
    /* tmp[0] holds LSB, tmp[pos-1] holds MSB */
    for (size_t i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    buf[pos] = '\0';
    return pos;
}

/* Build the reverse of the binary string of n, remove leading zeros,
   store in buf, and return its length. */
static size_t rev_no_leading(idx_t n, char *buf)
{
    char bin[128];
    size_t len = to_bin_string(n, bin);          /* len ≥ 1, bin[0] == '1' */

    int32_t pos = (int32_t)len - 1;              /* signed to allow -1 */
    while (pos >= 0 && bin[pos] == '0')
        --pos;                                  /* skip trailing zero(s) */

    size_t revlen = (pos + 1);                   /* number of digits */
    for (size_t i = 0; i < revlen; ++i)
        buf[i] = bin[pos - (int32_t)i];
    buf[revlen] = '\0';
    return revlen;
}

/* Return true if pattern b occurs inside string a. */
static bool str_contains(const char *a, const char *b)
{
    return strstr(a, b) != NULL;
}

seq_t a(idx_t n)
{
    /* Patterns for n and its reversed binary (without leading zeros). */
    char p[128], q[128];
    size_t lp = to_bin_string(n, p);
    size_t lq = rev_no_leading(n, q);

    /* Candidate 1: p followed by q, with maximal overlap. */
    char cand1[256];
    size_t cand1_len;
    if (str_contains(p, q)) {            /* q is already inside p */
        memcpy(cand1, p, lp);
        cand1_len = lp;
    } else {
        size_t ol = overlap_len(p, lp, q, lq);
        cand1_len = lp + lq - ol;
        memcpy(cand1, p, lp);
        memcpy(cand1 + lp, q + ol, lq - ol);
    }
    cand1[cand1_len] = '\0';

    /* Candidate 2: q followed by p, with maximal overlap. */
    char cand2[256];
    size_t cand2_len;
    if (str_contains(q, p)) {
        memcpy(cand2, q, lq);
        cand2_len = lq;
    } else {
        size_t ol = overlap_len(q, lq, p, lp);
        cand2_len = lq + lp - ol;
        memcpy(cand2, q, lq);
        memcpy(cand2 + lq, p + ol, lp - ol);
    }
    cand2[cand2_len] = '\0';

    /* Choose the shorter string, breaking ties lexicographically. */
    if (cand1_len < cand2_len || (cand1_len == cand2_len && strcmp(cand1, cand2) <= 0))
        return bin_to_seq(cand1);
    else
        return bin_to_seq(cand2);
}
