/*
 * A221373  A two-digit Look-and-Say sequence starting with 99: each term
 *          summarizes the increasing two-digit substrings of the previous
 *          term.
 * https://oeis.org/A221373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=99; for n>0, a(n) is obtained by
   (1) counting all overlapping two‑digit substrings of a(n-1),
   (2) for each two‑digit number x from 00 to 99 in increasing
       order, if it appears k times, appending the decimal
       representation of k followed by x (as two digits) to form a(n). */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../utils/seq_from_str.h"

static inline size_t digits_uint64(unsigned int x) {
    size_t d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n) {
    /* current term as a NUL‑terminated decimal string */
    char *curr = (char*)malloc(3);  /* "99" + '\0' */
    curr[0] = '9';
    curr[1] = '9';
    curr[2] = '\0';

    for (idx_t step = 1; step <= n; ++step) {
        size_t len = strlen(curr);
        unsigned int counts[100] = {0};

        /* count two‑digit substrings */
        for (size_t i = 0; i + 1 < len; ++i) {
            unsigned int v = (curr[i] - '0') * 10 + (curr[i+1] - '0');
            counts[v]++;
        }

        /* calculate length of next term */
        size_t new_len = 0;
        for (int v = 0; v < 100; ++v) {
            if (counts[v]) {
                new_len += digits_uint64(counts[v]) + 2;  /* count digits + two digits of v */
            }
        }

        char *next = (char*)malloc(new_len + 1);
        size_t pos = 0;
        char buf[32];

        for (int v = 0; v < 100; ++v) {
            if (counts[v]) {
                unsigned int cnt = counts[v];
                int sz = snprintf(buf, sizeof(buf), "%u", cnt);
                memcpy(next + pos, buf, sz);
                pos += sz;
                next[pos++] = '0' + v / 10;
                next[pos++] = '0' + v % 10;
            }
        }
        next[pos] = '\0';

        free(curr);
        curr = next;
    }

    seq_t result = seq_from_str(curr);
    free(curr);
    return result;
}
