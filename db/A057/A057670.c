/*
 * A057670  a(n) = Sum_{k|n} lcm(k, n/k).
 * https://oeis.org/A057670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Sum_{k|n} lcm(k, n/k) Multiplicative implementation:
 *            a(p^e) = Sum_{k=0..e} p^max(k, e-k)
 * Range:     u8 n <= 41, u16 n <= 2309, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types (idx_t, seq_t are provided) */

seq_t a(idx_t n) {
    if (n <= 1)          /* a(1) = 1, a(0) is irrelevant but handled */
        return (seq_t)n;

    seq_t result = 1;
    idx_t temp = n;

    /* factor by trial division */
    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p != 0)
            continue;

        /* exponent e of prime p in n */
        idx_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            ++e;
        }

        /* powers of p: p^0 .. p^e */
        /* array size (e+1) is safe for typical n <= 10^12 (e <= 40), but can grow if needed */
        seq_t powers[64];          /* assumes e < 64, adjust if larger exponents are expected */
        powers[0] = 1;
        for (idx_t i = 1; i <= e; ++i)
            powers[i] = powers[i - 1] * (seq_t)p;

        /* sum over k = 0..e of p^max(k, e-k) */
        seq_t sum = 0;
        for (idx_t k = 0; k <= e; ++k) {
            idx_t m = (k > e - k) ? k : e - k;
            sum += powers[m];
        }

        result *= sum;
    }

    /* if temp is a remaining prime factor > 1, exponent e = 1 */
    if (temp > 1) {
        seq_t p = (seq_t)temp;
        result *= (seq_t)2 * p;   /* sum for e = 1: p^1 + p^1 = 2p */
    }

    return result;
}
