/*
 * A075005  Floor[ concatenation of n+3, n+2, n+1 and n divided by 4 ].
 * https://oeis.org/A075005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(concat(n+3, n+2, n+1, n) / 4) (decimal
 *            concatenation)
 * Range:     u8 none, u16 n <= 8, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/* Parse a decimal string into a seq_t value.  Works for all allowed seq_t
   types because the arithmetic operators are defined for both
   machine integers and the big‑int class.                                 */
static seq_t seq_from_str(const char *s)
{
    seq_t v = 0;
    while (*s) { v = v*10 + (seq_t)(*s - '0'); ++s; }
    return v;
}

static void div_by_4(const char *in, char *out)
{
    int rem = 0;
    int outpos = 0;
    for (int i = 0; in[i]; ++i)
    {
        int digit = in[i] - '0';
        int cur = rem * 10 + digit;
        int q = cur / 4;
        rem = cur % 4;
        if (outpos > 0 || q > 0)
            out[outpos++] = '0' + q;
    }
    if (outpos == 0)
        out[outpos++] = '0';
    out[outpos] = '\0';
}

seq_t a(idx_t n)
{
    /* Build concatenation string of the four numbers */
    char buf[64], *p = buf;
    unsigned long long v;

    v = (unsigned long long)(n + 3);
    p += sprintf(p, "%llu", v);
    v = (unsigned long long)(n + 2);
    p += sprintf(p, "%llu", v);
    v = (unsigned long long)(n + 1);
    p += sprintf(p, "%llu", v);
    v = (unsigned long long)n;
    p += sprintf(p, "%llu", v);

    /* Divide that decimal string by 4, obtaining the quotient */
    char qbuf[64];
    div_by_4(buf, qbuf);

    /* Convert the quotient string back to seq_t */
    return seq_from_str(qbuf);
}
