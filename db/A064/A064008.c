/*
 * A064008  a(n) = (10^n - 1)/9*prime(n).
 * https://oeis.org/A064008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (10^n - 1)/9 * prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* offset is 1; behaviour for n < 1 is irrelevant */

    /* repunit: (10^n – 1)/9  */
    seq_t repunit = (pow10(n) - 1) / 9;

    /* nth prime (1‑indexed) */
    seq_t prime = (seq_t)get_prime(n);

    return repunit * prime;
}
