/*
 * A209466  Final digit of n^n - n.
 * https://oeis.org/A209466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n^n - n) mod 10
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   pow_last_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_last_digit.h"

static const unsigned char cycle_len[10] = {1,1,4,4,2,1,1,4,4,2};

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)1;  /* by definition a(0)=1 */

    unsigned char b = (unsigned char)(n % 10);
    unsigned char L = cycle_len[b];
    unsigned char exp = (unsigned char)(n % L);
    if (exp == 0)
        exp = L;

    unsigned char pow_mod = pow_last_digit(b, exp);
    unsigned char sub = (unsigned char)(n % 10);
    unsigned char result = (pow_mod + 10 - sub) % 10;
    return (seq_t)result;
}
