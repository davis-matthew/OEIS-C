/*
 * A166632  Totally multiplicative sequence with a(p) = 2*(p-1) for prime p.
 * https://oeis.org/A166632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, look
 * Formula:   a(n) = Π_{p^e‖n} (2*(p-1))^e (totally multiplicative,
 *            a(p)=2*(p-1))
 * Range:     u8 n <= 74, u16 n <= 5624, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;                      /* a(1)=1 */

    seq_t result = (seq_t)1;
    idx_t temp   = n;

    /* trial‑division factorisation */
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p != 0)
            continue;

        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }

        /* base = 2*(p-1) as seq_t */
        seq_t base = (seq_t)2 * ((seq_t)p - (seq_t)1);

        /* multiply result by base^exp   */
        result *= big_pow(base, (seq_t)exp);
    }

    /* remaining prime factor (if any) */
    if (temp > 1) {
        seq_t base = (seq_t)2 * ((seq_t)temp - (seq_t)1);
        result *= base;                     /* exp = 1 */
    }

    return result;
}
