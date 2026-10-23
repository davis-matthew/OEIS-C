/*
 * A179897  a(n) = (n^(2*n+1) + 1) / (n+1).
 * https://oeis.org/A179897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^(2*n+1) + 1) / (n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)(2*n + 1);
    seq_t pow = big_pow((seq_t)n, exp);
    pow += (seq_t)1;
    seq_t den = (seq_t)n + (seq_t)1;
    return pow / den;
}
