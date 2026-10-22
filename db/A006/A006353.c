/*
 * A006353  Expansion of (phi(-q^3) * psi(q))^3 / (phi(-q) * psi(q^3)) in
 *          powers of q where phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A006353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1; a(n) = Sum_{d|n} d*g(d) for n >= 1, where g(d) depends
 *            on d mod 6: g = 0,5,4,6,4,5 for d == 0,1,2,3,4,5 (mod 6)
 * Range:     u8 n <= 32, u16 n <= 5354, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const int g_tab[6] = {0, 5, 4, 6, 4, 5};

seq_t a(idx_t n) {
    seq_t s = 0;
    idx_t d;
    if (n == 0) return (seq_t)1;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            s += (seq_t)d * (seq_t)g_tab[d % 6];
            if (e != d) s += (seq_t)e * (seq_t)g_tab[e % 6];
        }
    }
    return s;
}
