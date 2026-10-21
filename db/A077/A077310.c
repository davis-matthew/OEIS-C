/*
 * A077310  Smallest multiple of n that is a concatenation of (increasing)
 *          natural numbers starting with(n+1).
 * https://oeis.org/A077310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 6, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   digits_of, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the smallest multiple of n that is a concatenation of
   increasing natural numbers starting with (n+1).  The algorithm scans
   successive end points t from n+1 upward, maintaining the value of
   the concatenation modulo n; when the modulo reaches zero the built
   string is converted to the arbitrary‑precision integer type seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../utils/digits_of.h"
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, safety only */

    seq_t mod = (seq_t)0;
    seq_t nn  = (seq_t)n;                 /* modulo base */

    /* Dynamic string builder for the concatenated number */
    size_t cap = 64;
    size_t len = 0;
    char *buf = (char *)malloc(cap);
    if (!buf) return (seq_t)0;
    buf[0] = '\0';

    for (idx_t t = n + 1; ; ++t) {
        /* Update modulo using the next integer t */
        int dt = digits_of(t);
        seq_t pow10 = 1;
        for (int i = 0; i < dt; ++i)
            pow10 = (pow10 * (seq_t)10) % nn;

        mod = (mod * pow10) % nn;
        mod += (seq_t)(t % n);
        mod %= nn;

        /* Append t to the string representation */
        char tmp[32];
        int tmp_len = sprintf(tmp, "%llu", (unsigned long long)t);
        if (len + tmp_len + 1 > cap) {
            cap = (cap + tmp_len + 1) * 2;
            char *newbuf = (char *)realloc(buf, cap);
            if (!newbuf) { free(buf); return (seq_t)0; }
            buf = newbuf;
        }
        memcpy(buf + len, tmp, tmp_len);
        len += tmp_len;
        buf[len] = '\0';

        if (mod == 0) {
            seq_t res = seq_from_str(buf);
            free(buf);
            return res;
        }
    }
}
