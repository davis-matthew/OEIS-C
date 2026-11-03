/*
 * A095263  a(n+3) = 3*a(n+2) - 2*a(n+1) + a(n), starting with (1, 3, 7).
 * https://oeis.org/A095263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Padovan(3n) = a(n) a(n+3) = 3*a(n+2) - 2*a(n+1) + a(n), a(1)=1,
 *            a(2)=3, a(3)=7 The Padovan sequence P satisfies:
 *            P(1)=P(2)=P(3)=1, P(k)=P(k-2)+P(k-3) for k≥4. Here a(n)=P(3n).
 *            Hence the values are obtained by computing Padovan(3*n)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 901 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t padovan_at(idx_t m)
{
    /* m ≥ 1, Padovan indices start at 1.  P(1)=P(2)=P(3)=1 */
    if (m <= 3)
        return (seq_t)1;
    seq_t p0 = (seq_t)1; /* P(i-3) */
    seq_t p1 = (seq_t)1; /* P(i-2) */
    seq_t p2 = (seq_t)1; /* P(i-1) */
    seq_t next = 0;
    for (idx_t i = 4; i <= m; ++i) {
        next = p1 + p0;          /* P(i) = P(i-2) + P(i-3) */
        p0 = p1;                 /* shift window */
        p1 = p2;
        p2 = next;
    }
    return p2; /* P(m) */
}

seq_t a(idx_t n)
{
    /* offset 1: a(1) is the first term */
    if (n == 0)
        return (seq_t)0; /* undefined, but return 0 for safety */
    /* compute Padovan(3n) */
    return padovan_at(3 * n);
}
