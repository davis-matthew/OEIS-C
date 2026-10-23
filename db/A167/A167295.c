/*
 * A167295  Totally multiplicative sequence with a(p) = 3*(p-2) for prime p.
 * https://oeis.org/A167295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} (3*(p-2))^e ; a(2k)=0 for k>=1
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build; do NOT redefine them. */

/* Computes x^e with e >= 0.  e is small for reasonable n. */
static seq_t pow_seq_exp(seq_t base, int e)
{
    seq_t r = 1;
    while (e > 0) {
        r *= base;
        e--;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1) return 1;

    /* Any factor 2 makes the value 0. */
    if ((n & 1) == 0) return 0;   /* n is even */

    seq_t result = 1;
    idx_t m = n;

    /* Trial‑division over odd candidates. */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t base = (seq_t)3 * (((seq_t)p) - 2);
            result *= pow_seq_exp(base, e);
        }
    }

    /* If m > 1 it is a remaining prime factor with exponent 1. */
    if (m > 1) {
        seq_t base = (seq_t)3 * (((seq_t)m) - 2);
        result *= base;
    }

    return result;
}
