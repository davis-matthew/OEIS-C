/*
 * A143100  A007318 * A143098.
 * https://oeis.org/A143100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - g(n), where g(n) = {2,0,-1,0,2,3} for n ≡
 *            0,1,2,3,4,5 (mod 6)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For completeness we return 0
       when n == 0, although this case does not appear in the OEIS data. */
    if (n == 0)
        return 0;

    /* g(n) depends only on n modulo 6.  The array below is indexed by
       n % 6.  Index 0 corresponds to n ≡ 0 (mod 6), 1 to n ≡ 1, etc. */
    static const int g_of_mod[6] = {2, 0, -1, 0, 2, 3};

    int g = g_of_mod[n % 6];

    /* 2^(n-1) is computed with the helper function pow2, which produces
       a seq_t value of the desired width. */
    seq_t value = pow2(n - 1);

    /* Subtract the (possibly negative) adjustment g.  Because seq_t
       is unsigned, we handle the sign explicitly. */
    if (g >= 0)
        value -= (seq_t)g;
    else
        value += (seq_t)(-g);

    return value;
}
