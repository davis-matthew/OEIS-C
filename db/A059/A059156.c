/*
 * A059156  A hierarchical sequence (W'3{2,2}c - see A059126).
 * https://oeis.org/A059156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum of A059148(3*n), A059148(3*n+1), A059148(3*n+2). Here
 *            A059148 produces a 3-term block (1,2,1) scaled by powers of 4
 *            where the scaling exponent depends on the 3‑ary position of n.
 *            Using trailing-ones logic the exponent e (so that a(n)=4^e) is:
 *            if floor(n/3) is even: e = (n%3==1) ? 2 : 1...
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   seq_pow, trailing_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/trailing_ones.h"

seq_t a(idx_t n)
{
    idx_t q = n / 3;
    unsigned r = (unsigned)(n % 3);
    unsigned e;
    if ((q & 1) == 0) {          /* q even */
        e = (r == 1) ? 2 : 1;
    } else {                     /* q odd */
        unsigned k = trailing_ones(q);
        unsigned base = 2 * k + 1;
        e = (r == 1) ? (base + 1) : base;
    }
    return seq_pow((seq_t)4, (idx_t)e);
}
