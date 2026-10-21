/*
 * A083655  Numbers which do not appear prematurely in the binary Champernowne
 *          word (A030190).
 * https://oeis.org/A083655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=2; for n>=3 let q = n/3 and r = n%3; if
 *            (r==0) a(n) = 2^(2q); if (r==1) a(n) = 2^(2q+1); if (r==2) a(n)
 *            = 2^(2q+1) + 2^q
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 47, u64 n <= 95, u128 n <= 113,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    idx_t q = n / 3u;
    int r = (int)(n % 3u);

    seq_t pow2_2q   = pow2(2 * q);
    seq_t pow2_2q1  = pow2(2 * q + 1);
    seq_t pow2_q    = pow2(q);

    switch (r) {
        case 0:
            return pow2_2q;
        case 1:
            return pow2_2q1;
        case 2:
            return pow2_2q1 + pow2_q;
    }
    /* unreachable, but satisfy compiler */
    return (seq_t)0;
}
