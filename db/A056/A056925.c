/*
 * A056925  Largest integer power of n which divides product of divisors of n.
 * https://oeis.org/A056925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^⌊τ(n)/2⌋, where τ(n) is the number of divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* simple fast exponentiation for any seq_t type */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1U) res *= base;
        base *= base;
        exp >>= 1U;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* compute τ(n) */
    idx_t temp = n;
    idx_t tau  = 1;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            tau *= (e + 1);
        }
    }
    if (temp > 1) tau *= 2;   /* trailing prime factor */

    /* exponent = ⌊τ(n)/2⌋ */
    idx_t exp = tau >> 1;
    return pow_seq((seq_t)n, exp);
}
