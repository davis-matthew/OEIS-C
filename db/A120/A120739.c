/*
 * A120739  a(n) = Sum_{k=0..n} floor(C(n,k)/2).
 * https://oeis.org/A120739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - A001316(n))/2, where A001316(n)=2^{popcount(n)}
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute popcount of n */
    idx_t pc = 0;
    for (idx_t t = n; t; t >>= 1) {
        pc += (t & 1);
    }

    /* For 64‑bit types and small n, compute with 128‑bit
       intermediates to avoid overflow of pow2 on large n. */
    if (sizeof(seq_t) <= 8 && n <= 65) {
        unsigned __int128 tot = ((unsigned __int128)1 << n) -
                               (((unsigned __int128)1) << pc);
        tot >>= 1;
        return (seq_t)tot;
    }

    /* Generic arbitrary‑precision implementation */
    seq_t p2n  = pow2(n);
    seq_t p2pc = pow2(pc);
    seq_t res  = p2n - p2pc;
    res >>= 1;               /* divide by 2 */
    return res;
}
