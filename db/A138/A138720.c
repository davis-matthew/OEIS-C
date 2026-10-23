/*
 * A138720  Concatenation of k digits 1, k digits 0 and k digits 1, where k is
 *          the n-th positive triangular number.
 * https://oeis.org/A138720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ((10^k - 1)/9) * (10^{2k} + 1), where k = n*(n+1)/2
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* k = n*(n+1)/2 as a seq_t */
    seq_t k = ((seq_t)n * ((seq_t)n + (seq_t)1)) / (seq_t)2;

    /* 10^k and 10^{2k} */
    seq_t pow10k   = big_pow((seq_t)10, k);
    seq_t pow102k  = big_pow((seq_t)10, k * (seq_t)2);

    /* number consisting of k ones */
    seq_t ones = (pow10k - (seq_t)1) / (seq_t)9;

    /* result: ones * (10^{2k} + 1) */
    return ones * (pow102k + (seq_t)1);
}
