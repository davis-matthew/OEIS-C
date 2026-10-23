/*
 * A193686  Lucas numbers (mod 100).
 * https://oeis.org/A193686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(n) mod 100 Lucas recurrence: L(0)=2, L(1)=1,
 *            L(n)=L(n-1)+L(n-2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod100
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mod100.h"

/* fast doubling to compute (F_n, F_{n+1}) modulo 100 */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    unsigned int a_ui = (unsigned int)a;
    unsigned int b_ui = (unsigned int)b;
    unsigned int two_b = (2u * b_ui) % 100u;

    /* F_{2k} = F_k * (2*F_{k+1} - F_k) */
    unsigned int c_val = (unsigned int)((a_ui * ((two_b + 100u - a_ui) % 100u)) % 100u);
    /* F_{2k+1} = F_k^2 + F_{k+1}^2 */
    unsigned int d_val = (unsigned int)((a_ui * a_ui + b_ui * b_ui) % 100u);

    if ((n & 1u) == 0u) {
        *fn = (seq_t)c_val;
        *fn1 = (seq_t)d_val;
    } else {
        *fn = (seq_t)d_val;
        *fn1 = (seq_t)((c_val + d_val) % 100u);
    }
}

seq_t a(idx_t n)
{
    /* Lucas number modulo 100, with offset 1 (a(1) = 1) */
    seq_t f0, f1;
    fib_pair(n, &f0, &f1);
    unsigned int f0_ui = (unsigned int)f0;
    unsigned int f1_ui = (unsigned int)f1;
    unsigned int l = (2u * f1_ui + 100u - f0_ui) % 100u;
    return (seq_t)l;
}
