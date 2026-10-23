/*
 * A115075  Number of 2 X 2 symmetric matrices over Z(n) having determinant 0.
 * https://oeis.org/A115075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = ∏_{p^e‖n} p^e·(p^e + p^(e-1) – p^(⌈e/2⌉-1)) For
 *            square‑free n this simplifies to a(n)=n^2
 * Range:     u8 n <= 15, u16 n <= 215, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t x = (uint64_t)n;
    seq_t result = (seq_t)1;

    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p != 0) continue;
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }

        /* compute p^e */
        seq_t pe = (seq_t)1;
        for (int i = 0; i < e; ++i) pe *= (seq_t)p;

        /* p^(e-1) */
        seq_t pe_prev = pe / (seq_t)p;

        /* p^(ceil(e/2)-1) */
        int ceil_half = (e + 1) / 2;      /* ceil(e/2) */
        seq_t p_ceil_half_prev = (seq_t)1;
        for (int i = 0; i < ceil_half - 1; ++i) p_ceil_half_prev *= (seq_t)p;

        seq_t factor = pe * ((pe + pe_prev) - p_ceil_half_prev);
        result *= factor;
    }

    /* If n had a prime factor > sqrt(n), it remains in x */
    if (x > 1) {
        /* x is prime, so e = 1 */
        seq_t p = (seq_t)x;
        seq_t pe = p;          /* p^1 */
        seq_t pe_prev = (seq_t)1;      /* p^0 */
        seq_t p_ceil_half_prev = (seq_t)1;  /* ceil(1/2) = 1 -> exponent 0 */
        seq_t factor = pe * ((pe + pe_prev) - p_ceil_half_prev);
        result *= factor;
    }

    return result;
}
