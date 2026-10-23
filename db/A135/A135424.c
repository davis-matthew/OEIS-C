/*
 * A135424  a(n) = n!^number of decimal digits of n!.
 * https://oeis.org/A135424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (n!)^(digit_count(n!))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, digit_len, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/digit_len.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);
    int d = digit_len(f);
    seq_t exp = (seq_t)d;
    return big_pow(f, exp);
}
