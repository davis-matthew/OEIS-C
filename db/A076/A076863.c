/*
 * A076863  a(n) = n^(n-2)+(n*(n-1)/2)*(n-1)^(n-3).
 * https://oeis.org/A076863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2)+(n*(n-1)/2)*(n-1)^(n-3)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the small indices directly to avoid negative exponents. */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t sn = (seq_t)n;
    seq_t first = big_pow(sn, (seq_t)(n - 2));

    /* Compute C(n,2) = n*(n-1)/2 */
    seq_t comb = sn * (sn - 1) / 2;

    seq_t second = big_pow(sn - 1, (seq_t)(n - 3));

    return first + comb * second;
}
