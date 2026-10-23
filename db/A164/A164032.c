/*
 * A164032  Number of "ON" cells in a certain 2-dimensional cellular
 *          automaton.
 * https://oeis.org/A164032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n%2==1?1:9)*4^wt(floor((n-1)/2))
 * Range:     u8 n <= 15, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset starts at 1 */
    seq_t c = (n & 1) ? (seq_t)1 : (seq_t)9;   /* c = 1 if odd, 9 if even */
    idx_t k = (n - 1) / 2;                 /* floor((n-1)/2) */
    idx_t w = __builtin_popcountll((unsigned long long)k); /* binary weight */
    seq_t pow4 = pow_int((seq_t)4, (idx_t)w);  /* 4^w */
    return c * pow4;
}
