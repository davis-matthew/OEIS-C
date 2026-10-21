/*
 * A067087  Concatenation of n-th prime and its reverse.
 * https://oeis.org/A067087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = concatenation of the n-th prime and its decimal reverse
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, get_prime, pow10, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/rev_decimal.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* offset 1: a(1) is the first term */

    uint32_t p   = get_prime(n);              /* n-th prime (uint32_t) */
    seq_t    rev = rev_decimal((seq_t)p);     /* decimal reverse of p */

    int      d   = digit_len((seq_t)p);       /* number of decimal digits of p */
    seq_t    p10 = pow10((idx_t)d);           /* 10^d */

    return ((seq_t)p) * p10 + rev;           /* concatenate prime and its reverse */
}
