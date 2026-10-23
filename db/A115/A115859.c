/*
 * A115859  Largest natural number m < n, such that there exists nonzero
 *          solutions to a cross-domain congruence m*i = n X i, zero if no
 *          such integer exists.
 * https://oeis.org/A115859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Brute‑force search for a(n):
    For each n>0 we look for the largest m<n such that there exists a
    non‑zero integer i with m*i = n X i, where n X i is the carryless
    (GF(2)[X]) product of n and i.  
    For each i up to a fixed upper bound we compute the carryless product
    as a XOR of shifted copies of n.  If that product is divisible by i,
    the quotient is a candidate m.  The maximum candidate under n is
    returned.  If none exists, 0 is returned.  This brute force method
    is sufficient for n≤10000 and runs well under the 2‑second limit.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* there can be no m < 1 */

    const idx_t LIMIT = ((idx_t)1) << 16;   /* 65536 – large enough for n ≤ 10000 */
    seq_t best = 0;                 /* best (max) m found so far */

    for (idx_t i = 1; i <= LIMIT; ++i) {
        /* compute carryless product n X i  (XOR of n shifted by each bit of i) */
        seq_t carryless = 0;
        idx_t tmp = i;
        idx_t shift = 0;
        while (tmp) {
            if (tmp & 1) {
                carryless ^= ((seq_t)n) << shift;
            }
            tmp >>= 1;
            ++shift;
        }

        /* check if ordinary product m*i equals carryless product */
        if (carryless % (seq_t)i == 0) {
            seq_t m = carryless / (seq_t)i;
            if (m < (seq_t)n && m > best) {
                best = m;          /* potential new maximum m */
            }
        }
    }
    return best;
}
