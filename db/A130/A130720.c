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
 * Helpers:   to_str_idx, to_str_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/to_str_idx.h"
#include "../utils/to_str_seq.h"

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
