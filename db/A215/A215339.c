/*
 * A215339  a(n) = A001608(n) mod n.
 * https://oeis.org/A215339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Perrin(n) mod n Perrin sequence: P(0)=3, P(1)=0, P(2)=2,
 *            P(k)=P(k-2)+P(k-3) (k≥3)
 * Range:     u8 n <= 284, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n≥1 by definition; for n=1,2 return 0 */
    if (n <= 2)
        return 0;

    /* Work modulo n so all intermediate values stay < n */
    seq_t mod = (seq_t)n;

    seq_t p0 = (seq_t)3 % mod; /* P(0) mod n */
    seq_t p1 = (seq_t)0 % mod; /* P(1) mod n */
    seq_t p2 = (seq_t)2 % mod; /* P(2) mod n */

    /* Iterate from k = 3 up to n */
    for (idx_t k = 3; ; ++k) {
        seq_t p3 = (p1 + p0) % mod;   /* P(k) mod n */

        if (k == n)
            return p3;

        /* Shift the window for next iteration */
        p0 = p1;
        p1 = p2;
        p2 = p3;
    }
}
