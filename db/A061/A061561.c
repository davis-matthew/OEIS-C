/*
 * A061561  Trajectory of 22 under the Reverse and Add! operation carried out
 *          in base 2.
 * https://oeis.org/A061561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 22, a(1) = 35, for n >= 2: let r = n mod 4. if r == 0: k
 *            = (n+4)/4, a(n) = 12*2^(2k) - 3*2^k if r == 1: k = (n+3)/4, a(n)
 *            = 12*2^(2k) + 9*2^k - 3 if r == 2: k = (n+6)/4, a(n) = 6*2^(2k)
 *            - 3*2^k if r == 3: k = (n+5)/4, a(n) = 6*2^(2k) + 3*2^k - 3
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 53, u64 n <= 117, u128 n <= 245,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)22;
    if (n == 1)
        return (seq_t)35;

    idx_t r = n % 4;
    idx_t k;
    seq_t term;

    switch (r) {
        case 0:   /* n ≡ 0 (mod 4) */
            k = (n + 4) / 4;
            term = 12ull * pow_uint(2, 2 * k) - 3ull * pow_uint(2, k);
            break;
        case 1:   /* n ≡ 1 (mod 4) */
            k = (n + 3) / 4;
            term = 12ull * pow_uint(2, 2 * k) + 9ull * pow_uint(2, k) - 3ull;
            break;
        case 2:   /* n ≡ 2 (mod 4) */
            k = (n + 6) / 4;
            term = 6ull * pow_uint(2, 2 * k) - 3ull * pow_uint(2, k);
            break;
        case 3:   /* n ≡ 3 (mod 4) */
            k = (n + 5) / 4;
            term = 6ull * pow_uint(2, 2 * k) + 3ull * pow_uint(2, k) - 3ull;
            break;
        default:  /* should never happen */
            term = 0;
            break;
    }

    return term;
}
