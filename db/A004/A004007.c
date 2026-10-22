/*
 * A004007  Theta series of E_6 lattice.
 * https://oeis.org/A004007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{d|n} (81*chi(n/d) - 9*chi(d)) * d^2 for n
 *            >= 1, where chi(m) = Kronecker(-3, m), i.e. 0, 1, -1 for m == 0,
 *            1, 2 (mod 3)
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   chi3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/chi3.h"

static void add_term(seq_t *pos, seq_t *neg, idx_t n, idx_t d)
{
    seq_t d2 = (seq_t)d * (seq_t)d;
    int c1 = chi3(n / d);
    int c2 = chi3(d);
    if (c1 > 0) *pos += d2 * 81;
    else if (c1 < 0) *neg += d2 * 81;
    if (c2 > 0) *neg += d2 * 9;
    else if (c2 < 0) *pos += d2 * 9;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t d;
    if (n == 0) return (seq_t)1;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            add_term(&pos, &neg, n, d);
            if (d != n / d) add_term(&pos, &neg, n, n / d);
        }
    }
    return pos - neg;
}
