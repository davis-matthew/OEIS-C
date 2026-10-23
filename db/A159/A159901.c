/*
 * A159901  Concatenation of the first n squarefree numbers A005117.
 * https://oeis.org/A159901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = concatenation of the first n squarefree numbers A005117
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

/* Returns true if x is squarefree (no prime divides x with exponent >= 2). */
static bool is_squarefree_uint64(uint64_t x)
{
    for (uint64_t j = 2; j <= x / j; ++j) {
        uint64_t sq = j * j;
        if (x % sq == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* Buffer to accumulate decimal digits of the concatenation. */
    size_t bufsize = 256;
    char *buf = (char *)malloc(bufsize);
    if (!buf)
        return seq_from_str("0");          /* unlikely to happen */
    size_t len = 0;

    uint64_t count = 0;      /* number of squarefree numbers found */
    for (uint64_t num = 1; ; ++num) {
        if (is_squarefree_uint64(num)) {
            /* Convert num to string. */
            char tmp[32];
            int tlen = snprintf(tmp, sizeof(tmp), "%llu",
                                (unsigned long long)num);
            if (len + tlen + 1 > bufsize) {    /* +1 for null terminator */
                bufsize = (bufsize + tlen + 1) * 2;
                char *newbuf = (char *)realloc(buf, bufsize);
                if (!newbuf) {
                    free(buf);
                    return seq_from_str("0");
                }
                buf = newbuf;
            }
            memcpy(buf + len, tmp, tlen);
            len += tlen;
            buf[len] = '\0';

            ++count;
            if (count == (uint64_t)n)
                break;
        }
    }

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
