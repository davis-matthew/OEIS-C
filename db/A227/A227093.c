/*
 * A227093  Decimal expansion of 1/9899.
 * https://oeis.org/A227093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/9899) % 10 (the n‑th digit of 1/9899 after
 *            the decimal point)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Modulus 10*9899 = 98990, fits easily in seq_t  */
    seq_t mod = 98990;
    /* Compute 10^(n+1) mod mod */
    seq_t rem = mod_pow(10, n + 1, mod);
    /* The digit is floor(rem / 9899) because rem < 10*9899 */
    return rem / 9899;
}
