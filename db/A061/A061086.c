/*
 * A061086  a(n) is the concatenation of n with n^3.
 * https://oeis.org/A061086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of n and n^3
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast the index to seq_t */
    seq_t n_seq = (seq_t)n;

    /* compute n^3 */
    seq_t n_sq = n_seq * n_seq;
    seq_t n_cu = n_sq * n_seq;

    /* compute the number of decimal digits of n^3 */
    idx_t digits = 0;
    seq_t tmp = n_cu;
    seq_t zero = (seq_t)0;
    while (tmp > zero)
    {
        tmp = tmp / (seq_t)10;
        ++digits;
    }

    /* compute 10^digits */
    seq_t pow10 = (seq_t)1;
    for (idx_t i = 0; i < digits; ++i)
        pow10 *= (seq_t)10;

    /* concatenation: n * 10^digits + n^3 */
    return n_seq * pow10 + n_cu;
}
