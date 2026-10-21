/*
 * A068824  a(n) = n*c(n), where c(n) is the 10's complement of n.
 * https://oeis.org/A068824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n * (10^digits(n) - n)
 * Range:     u8 n <= 9, u16 n <= 99, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* digits(n) is the number of decimal digits of n */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1; behaviour for n==0 not important */

    seq_t nseq = (seq_t)n;      /* promote index to sequence type */

    /* count digits of n */
    idx_t temp   = n;
    idx_t digits = 0;
    do
    {
        digits++;
        temp /= 10;
    } while (temp);

    /* compute 10^digits */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < digits; ++i)
        pow10 *= 10;

    /* complement and product */
    seq_t comp = pow10 - nseq;
    return nseq * comp;
}
