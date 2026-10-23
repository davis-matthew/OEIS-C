/*
 * A175841  Fast "exotic addition" a o b = [ a[1]+b[1], a[1]*b[2]+a[2]*b[1] ].
 * https://oeis.org/A175841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(1)=1; a(2k)=2k*a(k); a(2k+1)=2k*(1+a(k))
 * Range:     u8 n <= 11, u16 n <= 39, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Recursive helper that follows the instructions of the exotic addition */
static seq_t _a_rec(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;                    /* base case */

    if ((n & (idx_t)1) == 0) {             /* n is even: n = 2k */
        idx_t k = n >> 1;
        seq_t ak = _a_rec(k);
        return ((seq_t)2 * (seq_t)k) * ak;
    } else {                               /* n is odd: n = 2k+1 */
        idx_t k = n >> 1;
        seq_t ak = _a_rec(k);
        return ((seq_t)2 * (seq_t)k) * ((seq_t)1 + ak);
    }
}

/* Required external linkage function */
seq_t a(idx_t n)
{
    return _a_rec(n);
}
