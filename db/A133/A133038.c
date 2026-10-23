/*
 * A133038  Cubes of A000931.
 * https://oeis.org/A133038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Padovan(n)^3; Padovan: P(0)=1, P(1)=0, P(2)=0,
 *            P(n)=P(n-2)+P(n-3)
 * Range:     u8 n <= 12, u16 n <= 19, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t p0 = (seq_t)1; /* P(0) */
    seq_t p1 = (seq_t)0; /* P(1) */
    seq_t p2 = (seq_t)0; /* P(2) */

    if (n == 0) {
        seq_t t = seq_mul(p0, p0);
        return seq_mul(t, p0);
    }
    if (n == 1) {
        seq_t t = seq_mul(p1, p1);
        return seq_mul(t, p1);
    }
    if (n == 2) {
        seq_t t = seq_mul(p2, p2);
        return seq_mul(t, p2);
    }

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = p0 + p1;   /* P(i-3) + P(i-2) */
        p0 = p1;
        p1 = p2;
        p2 = next;
    }

    seq_t t = seq_mul(p2, p2);
    return seq_mul(t, p2);
}
