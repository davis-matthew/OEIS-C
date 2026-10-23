/*
 * A100656  a(n)=1 if a hexagonal number is a prime, otherwise 0.
 * https://oeis.org/A100656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if 3*n*(n+1)+1 (a centered hexagonal number) is prime,
 *            otherwise 0
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_seq.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t hex = 3 * x * (x + 1) + 1;
    return is_prime_seq(hex) ? 1 : 0;
}
