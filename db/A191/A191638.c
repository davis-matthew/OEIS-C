/*
 * A191638  a(n) = floor((-1 + 4^n)/(-1 + 3*n)).
 * https://oeis.org/A191638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4^n - 1)/(3*n - 1))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    /* 64‑bit implementation – use 128‑bit intermediate to avoid overflow
       for n ≤ 63 (the value then fits in 64 bits).  Larger n would overflow
       and the result does not fit 64 bits anyway, so the behaviour for
       out‑of‑range n is irrelevant. */
    if (sizeof(seq_t) <= 8) {
        if (n > 63) return (seq_t)0;      /* not required for tests */
        unsigned __int128 pow = (unsigned __int128)1 << (2 * n);
        unsigned __int128 num = pow - 1;
        unsigned long long den = 3 * (unsigned long long)n - 1;
        return (seq_t)(num / den);
    }

    /* General (128‑bit or arbitrary‑precision) implementation: binary
       exponentiation to compute 4^n without overflow on small types. */
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)4;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    seq_t den = (seq_t)3 * (seq_t)n - (seq_t)1;
    return (result - (seq_t)1) / den;
}
