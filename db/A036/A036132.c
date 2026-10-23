/*
 * A036132  a(n) = 7^n mod 71.
 * https://oeis.org/A036132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 71
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Reduce the exponent modulo 70 by Fermat's little theorem
       (71 is prime, so 7^70 ≡ 1 (mod 71)).  This keeps the
       intermediate numbers tiny. */
    idx_t e = n % 70;
    seq_t res = (seq_t)1;
    const seq_t base = (seq_t)7;
    const seq_t mod  = (seq_t)71;

    for (idx_t i = 0; i < e; ++i)
        res = (res * base) % mod;

    return res;
}
