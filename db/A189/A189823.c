/*
 * A189823  Decimal expansion of the reduced Champernowne constant.
 * https://oeis.org/A189823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   not stated
 * Range:     u8 n <= 9986, u16 n <= 9986, u32 n <= 9986, u64 n <= 9986,
 *            u128 n <= 9986, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th digit (0‑indexed) of the reduced Champernowne constant,
   constructed by concatenating successive integers that are not already
   substrings of the sequence. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static char *buf   = NULL;   /* accumulating digit string */
    static size_t len  = 0;      /* actual number of digits in buf */
    static size_t cap  = 0;      /* allocated buffer size */
    static uint64_t next = 1;    /* next integer to consider */
    static int initialized = 0;

    if (!initialized) {
        cap = 1024;
        buf = (char *)malloc(cap);
        if (!buf) abort();
        buf[0] = '\0';
        initialized = 1;
    }

    /* Ensure we have generated at least n+1 digits */
    while (len <= (size_t)n) {
        /* Convert next integer to a decimal string in reverse order */
        char s[32];
        uint64_t x = next++;
        int slen = 0;
        do {
            s[slen++] = (char)('0' + (x % 10));
            x /= 10;
        } while (x);

        /* Reverse to obtain correct order */
        for (int i = 0, j = slen - 1; i < j; ++i, --j) {
            char tmp = s[i];
            s[i] = s[j];
            s[j] = tmp;
        }
        /* Null‑terminate the temporary string for safe strstr */
        s[slen] = '\0';

        /* Append only if the integer string is not already present */
        if (strstr(buf, s) == NULL) {
            /* Grow buffer if necessary (include space for terminating NUL) */
            if (len + slen + 1 > cap) {
                size_t newcap = cap ? cap << 1 : 1024;
                while (newcap < len + slen + 1)
                    newcap <<= 1;
                char *tmp = (char *)realloc(buf, newcap);
                if (!tmp) abort();
                buf = tmp;
                cap = newcap;
            }
            memcpy(buf + len, s, slen);
            len += slen;
            buf[len] = '\0';
        }
    }

    return (seq_t)(buf[(size_t)n] - '0');
}
