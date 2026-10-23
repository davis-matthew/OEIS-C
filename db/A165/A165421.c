/*
 * A165421  a(1) = 1, a(2) = 3, a(n) = product of the previous terms for n >=
 *          3.
 * https://oeis.org/A165421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n=1; 3 for n=2; a(n)=∏_{i=1}^{n-1} a(i) for n≥3
 *            Equivalently, a(1)=1, a(2)=3, a(n)=a(n‑1)² for n≥4
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    /* exponent = 2^(n-3) */
    seq_t exp = pow2((idx_t)(n - 3));
    seq_t base = (seq_t)3;
    return big_pow(base, exp);
}
