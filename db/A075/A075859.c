/*
 * A075859  a(n) = n concatenated with n 1's and n.
 * https://oeis.org/A075859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   a(n) = concat(n, 111...1 (n times), n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   repunit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/repunit.h"
static seq_t pow10_exp(idx_t e)
{
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i) {
        result *= 10;
    }
    return result;
}

static unsigned decimal_digits(idx_t n)
{
    unsigned d = 0;
    do {
        ++d;
        n /= 10;
    } while (n != 0);
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* not used, but keeps behaviour sane */
    unsigned L = decimal_digits(n);               /* number of decimal digits of n */
    seq_t pow10L   = pow10_exp((idx_t)L);         /* 10^L                           */
    seq_t pow10nL  = pow10_exp((idx_t)(n + L));   /* 10^(n+L)                       */
    seq_t R        = repunit(n);                  /* repunit of length n            */
    seq_t result   = (seq_t)n * pow10nL + pow10L * R + (seq_t)n;
    return result;
}
