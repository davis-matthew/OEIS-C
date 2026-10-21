/*
 * A088891  Polynexus numbers of order 9.
 * https://oeis.org/A088891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ((n^9-(n-1)^9) - (n^3-(n-1)^3)) / 504 (where 504 =
 *            2^9-2^3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 26, u64 n <= 423, u128 n <= 1000,
 *            big any n
 * Helpers:   pow_seq_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_u.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defined a(0)=0, offset 1 */
    /* Branch on whether seq_t is a small builtin integer */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
    {
        __int128 nn   = (__int128)n;
        __int128 n1   = nn - 1;
        __int128 n9   = 1, n1_9 = 1;
        __int128 n3   = 1, n1_3 = 1;

        for (int i = 0; i < 9; ++i) n9   *= nn;
        for (int i = 0; i < 9; ++i) n1_9 *= n1;
        for (int i = 0; i < 3; ++i) n3   *= nn;
        for (int i = 0; i < 3; ++i) n1_3 *= n1;

        __int128 numer = (n9 - n1_9) - (n3 - n1_3);
        __int128 res   = numer / 504;
        return (seq_t)res;
    }
    else /* seq_t is a big‑integer type */
    {
        seq_t nseq = (seq_t)n;
        seq_t n1seq = nseq - 1;

        seq_t diff9 = pow_seq_u(nseq, 9) - pow_seq_u(n1seq, 9);
        seq_t diff3 = pow_seq_u(nseq, 3) - pow_seq_u(n1seq, 3);

        seq_t numer = diff9 - diff3;
        seq_t denom = (seq_t)504;
        return numer / denom;
    }
}
