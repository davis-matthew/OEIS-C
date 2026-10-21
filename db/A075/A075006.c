/*
 * A075006  Floor[ concatenation of n+4, n+3, n+2, n+1 and n divided by 5].
 * https://oeis.org/A075006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor( concat(n+4, n+3, n+2, n+1, n) / 5 )
 * Range:     u8 none, u16 n <= 2, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)(n + 4);
    res = res * pow10((idx_t)digits(n + 3)) + (seq_t)(n + 3);
    res = res * pow10((idx_t)digits(n + 2)) + (seq_t)(n + 2);
    res = res * pow10((idx_t)digits(n + 1)) + (seq_t)(n + 1);
    res = res * pow10((idx_t)digits(n))    + (seq_t)(n);
    return res / (seq_t)5;
}
