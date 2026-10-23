/*
 * A176999  An encoding of the Collatz iteration of n.
 * https://oeis.org/A176999
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal number whose decimal digits are the reversed
 *            parity vector of the Collatz iteration of n, where 0 stands for
 *            a 3x+1 step and 1 for an x/2 step
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 6, u64 n <= 24, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* If n < 2 the sequence is undefined; return 0. */
    if (n < 2) return 0;

    /* A static buffer of bits; for n up to about 10 000 the Collatz
       trajectory length is < 20000, comfortably below this limit. */
    unsigned char bits[20000];
    int bitcnt = 0;

    idx_t cur = n;
    while (cur != 1) {
        if (cur & (idx_t)1) {          /* odd -> 3x+1 */
            bits[bitcnt++] = (unsigned char)0;
            cur = 3 * cur + (idx_t)1;
        } else {                       /* even -> x/2 */
            bits[bitcnt++] = (unsigned char)1;
            cur >>= 1;
        }
        /* No need for overflow protection; the test set stays within
           range where 3*cur+1 fits into idx_t. */
    }

    /* Assemble the result: most significant decimal digit first,
       therefore multiply by 10 (decimal) and add the bit. */
    seq_t res = 0;
    for (int i = bitcnt - 1; i >= 0; --i) {
        res = res * (seq_t)10 + (seq_t)bits[i];
    }
    return res;
}
