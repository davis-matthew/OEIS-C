/*
 * A227047  Expansion of x^2*(1+x^2) / ( (x^2-x+1)*(-x^2-x+1)*(1+x+x^2) ).
 * https://oeis.org/A227047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = a(n-1)+a(n-3)+a(n-5)+a(n-6) with initial values a(0)=0,
 *            a(1)=0, a(2)=1, a(3)=1, a(4)=2, a(5)=3, a(6)=4
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases for n ≤ 6 */
    static const seq_t base[7] = { (seq_t)0, (seq_t)0, (seq_t)1,
                                   (seq_t)1, (seq_t)2, (seq_t)3, (seq_t)4 };
    if (n <= 6u) return base[(size_t)n];

    /* A small ring buffer holding the 7 most recent values */
    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i) buf[i] = base[i];

    idx_t s = 0;                     /* buf[s] is a(current‑6) */
    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = a(i‑1) + a(i‑3) + a(i‑5) + a(i‑6) */
        seq_t next = buf[(s + 6) % 7] + buf[(s + 4) % 7]
                   + buf[(s + 2) % 7] + buf[(s + 1) % 7];
        s = (s + 1) % 7;                 /* advance window */
        buf[(s + 6) % 7] = next;          /* store a(i) */
    }

    return buf[(s + 6) % 7];
}
