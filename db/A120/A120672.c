/*
 * A120672  a(n) = 2 * A285917(n) for n >=2, a(0) = a(1) = 0.
 * https://oeis.org/A120672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(2^n - 2) - (n even ? binom(n,n/2) : 0) for n ≥ 2; a(0)
 *            = a(1) = 0
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t binom_choose(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = (res * (n - k + i)) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1u)
        return (seq_t)0;

    seq_t val = (seq_t)2 * (pow2(n) - (seq_t)2);

    if ((n & 1u) == 0u) /* n even */
        val -= binom_choose(n, n >> 1);

    return val;
}
