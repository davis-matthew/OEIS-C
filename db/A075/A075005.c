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
 * Helpers:   div_by_4, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/div_by_4.h"

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
