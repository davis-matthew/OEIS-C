/*
 * A143222  a(0)=0. For n >=1, a(n) = 0 if the binary representation of n
 *          occurs at least once in the concatenation of
 *          (a(0),a(1),...,a(n-1)). a(n) = 1 otherwise.
 * https://oeis.org/A143222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 0 if the binary representation of n occurs in the
   concatenation of a(0)…a(n‑1); otherwise a(n)=1.
   a(0)=0. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Cache of computed sequence values. */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Buffer holding the concatenation of the binary strings of the
   terms we have generated so far (as characters '0' or '1'). */
static char *bit_buf = NULL;
static size_t buf_cap = 0;
static size_t buf_len = 0;

/* Ensure seq_cache can hold indices up to n (inclusive).  n is
   treated as an idx_t; ensure_cache guarantees capacity of n+1. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n + 1) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* Ensure bit_buf can store at least sz+1 characters (including the
   terminating NUL).  sz is the number of characters currently in the
   buffer. */
static void ensure_buf(size_t sz)
{
    if (sz + 1 <= buf_cap) return;
    size_t new_cap = buf_cap ? buf_cap * 2 : 1;
    while (new_cap <= sz + 1) new_cap <<= 1;
    char *tmp = (char *)realloc(bit_buf, new_cap);
    if (!tmp) exit(1);
    bit_buf = tmp;
    buf_cap = new_cap;
}

/* Append a single character (normally '0' or '1') to bit_buf. */
static void buf_append(char c)
{
    ensure_buf(buf_len);
    bit_buf[buf_len++] = c;
    bit_buf[buf_len] = '\0';
}

/* Convert n to a binary string with no leading zeros.
   The string is written into out, and its length is returned in out_len. */
static void bin_str(idx_t n, char *out, size_t *out_len)
{
    if (n == 0) {
        out[0] = '0';
        *out_len = 1;
        return;
    }
    char tmp[65];
    int pos = 0;
    idx_t t = n;
    while (t) {
        tmp[pos++] = (char)('0' + (int)(t & 1));
        t >>= 1;
    }
    for (int i = 0; i < pos; ++i) out[i] = tmp[pos - 1 - i];
    *out_len = (size_t)pos;
}

/* Very small substring search.  len_needle can be zero, in which case
   the function returns true. */
static bool contains(const char *hay, size_t hlen, const char *needle, size_t needle_len)
{
    if (needle_len == 0) return true;
    for (size_t i = 0; i + needle_len <= hlen; ++i) {
        if (hay[i] == needle[0] && memcmp(&hay[i], needle, needle_len) == 0)
            return true;
    }
    return false;
}

/* Compute the sequence value at index n.  The function is safe to use
   in any call order and memoizes results for efficiency. */
seq_t a(idx_t n)
{
    /* First, make sure the cache and bit buffer can hold all needed
       values.  We do this once for the entire range up to n. */
    ensure_cache(n);
    ensure_buf(0);          /* make sure buffer is not NULL */

    /* If we already know the value, just return it. */
    if ((size_t)n < cache_len) return seq_cache[n];

    /* Initialise the cache for the very first element, if we haven't
       already done so. */
    if (cache_len == 0) {
        seq_cache[0] = (seq_t)0;
        buf_append('0');      /* representation of a(0) is "0" */
        cache_len = 1;
    }

    /* Generate missing terms up to n.  Each new value is appended to
       the cache and the bit buffer. */
    for (idx_t i = cache_len; i <= n; ++i) {
        char bin[65];
        size_t binlen;
        bin_str(i, bin, &binlen);
        seq_t val = contains(bit_buf, buf_len, bin, binlen) ? (seq_t)0 : (seq_t)1;
        seq_cache[i] = val;
        buf_append((char)('0' + (int)val));
    }

    cache_len = (size_t)n + 1;
    return seq_cache[n];
}
