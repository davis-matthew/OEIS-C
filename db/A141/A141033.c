/*
 * A141033  Numbers that cannot be formed by the concatenation of previous
 *          terms.
 * https://oeis.org/A141033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS A141033: numbers that cannot be formed by the
   concatenation of distinct previous terms.
   For n >= 1, a(1)=0, a(2)=1, ... a(10)=9, a(12)=22, ...
   The algorithm generates terms in increasing order and skips a
   candidate if any partition of its decimal representation into
   distinct existing terms is possible.
   Leading zeros are forbidden in any part.
*/

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CAP 50000   /* enough for n up to about 10 000 */

/* ------------------------------------------------------------------ */
/* Simple sequential storage of the sequence (works in C and C++)      */
/* ------------------------------------------------------------------ */
static seq_t seq_data[MAX_CAP];
static size_t seq_len = 0;

static inline size_t seq_size(void) { return seq_len; }
static inline void seq_append(seq_t val) { if (seq_len < MAX_CAP) seq_data[seq_len++] = val; }
static inline seq_t seq_get(size_t idx) { return seq_data[idx]; }

/* ------------------------------------------------------------------ */
/* Binary search: index of value in the sequence, -1 if not found     */
/* ------------------------------------------------------------------ */
static inline int seq_index(seq_t val) {
    size_t lo = 0, hi = seq_size();
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        seq_t midv = seq_get(mid);
        if (midv < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < seq_size() && seq_get(lo) == val)
        return (int)lo;
    return -1;
}

/* ------------------------------------------------------------------ */
/* Convert decimal integer to a string (buffer must hold enough space) */
/* ------------------------------------------------------------------ */
static inline void seq_to_str(seq_t x, char *buf, size_t *len_out) {
    if (x == (seq_t)0) {
        buf[0] = '0';
        *len_out = 1;
        return;
    }
    size_t pos = 0;
    while (x != (seq_t)0) {
        unsigned digit = (unsigned)(x % 10);
        buf[pos++] = '0' + digit;
        x /= 10;
    }
    /* reverse the buffer */
    for (size_t i = 0; i < pos / 2; ++i) {
        char tmp = buf[i];
        buf[i] = buf[pos - 1 - i];
        buf[pos - 1 - i] = tmp;
    }
    *len_out = pos;
}

/* ------------------------------------------------------------------ */
/* Recursive search for a valid partition of the string representation */
/* ------------------------------------------------------------------ */
static bool _dfs(const char *s, size_t slen, bool *used, size_t pos) {
    if (pos == slen) return true;              /* all digits consumed */

    seq_t cur = 0;
    for (size_t i = pos; i < slen; ++i) {
        /* reject substrings with a leading zero unless they are "0" */
        if (i > pos && s[pos] == '0')
            break;   /* further extensions would also start with '0' */

        unsigned digit = (unsigned)(s[i] - '0');
        cur = cur * 10 + digit;

        int idx = seq_index(cur);
        if (idx >= 0 && !used[idx]) {
            used[idx] = true;
            if (_dfs(s, slen, used, i + 1))
                return true;
            used[idx] = false;
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Determine whether candidate can be expressed by earlier terms      */
/* ------------------------------------------------------------------ */
static bool can_form(seq_t candidate) {
    if (seq_size() == 0) return false;          /* no previous terms */

    char buf[64];
    size_t len;
    seq_to_str(candidate, buf, &len);

    static bool used[MAX_CAP];
    memset(used, 0, seq_size() * sizeof(bool));

    return _dfs(buf, len, used, 0);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n) {
    static seq_t candidate = (seq_t)0;
    while (seq_size() < (size_t)n) {
        if (!can_form(candidate))
            seq_append(candidate);
        ++candidate;
    }
    return seq_get((size_t)n - 1);
}
