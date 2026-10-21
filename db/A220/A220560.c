/*
 * A220560  Number of ways to reciprocally link elements of an nX6 array
 *          either to themselves or to exactly one horizontal or antidiagonal
 *          neighbor.
 * https://oeis.org/A220560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 43*a(n-1) -317*a(n-2) +783*a(n-3) -721*a(n-4) +375*a(n-5)
 *            -125*a(n-6) +19*a(n-7) - a(n-8) for n>9 Initial terms: a(1)=13,
 *            a(2)=481, a(3)=16310, a(4)=561636, a(5)=19333688,
 *            a(6)=665748170, a(7)=22926570957, a(8)=789539775889,
 *            a(9)=27190037181789
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[9] = {
    (seq_t)13ULL,
    (seq_t)481ULL,
    (seq_t)16310ULL,
    (seq_t)561636ULL,
    (seq_t)19333688ULL,
    (seq_t)665748170ULL,
    (seq_t)22926570957ULL,
    (seq_t)789539775889ULL,
    (seq_t)27190037181789ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0 */
    if (n <= 9) return init[n-1];

    /* buf[0]..buf[8] hold the last nine terms in order:
       buf[0]=a(n-8), buf[1]=a(n-7), … , buf[8]=a(n-1)   */
    seq_t buf[9];
    for (int i = 0; i < 9; ++i) buf[i] = init[i];

    for (idx_t i = 10; i <= n; ++i) {
        seq_t val = (seq_t)43 * buf[8];
        val -= (seq_t)317 * buf[7];
        val += (seq_t)783 * buf[6];
        val -= (seq_t)721 * buf[5];
        val += (seq_t)375 * buf[4];
        val -= (seq_t)125 * buf[3];
        val += (seq_t)19  * buf[2];
        val -= buf[1];

        /* shift the window left */
        for (int k = 0; k < 8; ++k) buf[k] = buf[k+1];
        buf[8] = val;
    }
    return buf[8];
}
