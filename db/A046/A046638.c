/*
 * A046638  Number of cubic residues mod 10^n, or number of distinct n-digit
 *          endings of cubes.
 * https://oeis.org/A046638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A046638: Number of cubic residues modulo 10^n
   a(n) = A046630(n) * A046633(n)
   where
   A046630(n) = 1 + Σ_{t=0}^{⌊(n-1)/3⌋} φ(2^{n-3t})            (n≥1)
   A046633(n) = 1 + Σ_{t=0}^{⌊(n-1)/3⌋} φ(5^{n-3t})            (n≥1)
   φ(2^k) = 2^{k-1}   (k≥1),   φ(5^k) = 4·5^{k-1}   (k≥1)
   For n=0, a(0)=1. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    idx_t tmax = (n - 1) / 3;

    seq_t sum2 = 0;
    seq_t sum5 = 0;

    for (idx_t t = 0; t <= tmax; ++t) {
        idx_t k = n - 3 * t;               /* exponent for the prime power */
        seq_t exp = (seq_t)(k - 1);        /* k-1 >= 0 */

        /* φ(2^k) = 2^{k-1} */
        seq_t phi2 = big_pow((seq_t)2, exp);
        sum2 += phi2;

        /* φ(5^k) = 4·5^{k-1} */
        seq_t phi5 = 4 * big_pow((seq_t)5, exp);
        sum5 += phi5;
    }

    /* add the zero residue for each modulus */
    sum2 += (seq_t)1;
    sum5 += (seq_t)1;

    return sum2 * sum5;
}
