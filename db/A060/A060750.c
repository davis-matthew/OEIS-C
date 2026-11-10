/*
 * A060750  Step at which card n appears on top of deck for first time in
 *          Guy's shuffling problem A035485.
 * https://oeis.org/A060750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = least k >= 1 such that card n is on top after k shuffles.
 *            * Computed by simulating the card's position: at step k a card
 *            at position * p <= 2k moves to 2p mod (2k+1) (in 1..2k); a card
 *            at p > 2k does not move. * The opening steps where the card does
 *            not move are skipped, and the loop * run...
 * Range:     u8 n <= 12, u16 n <= 53, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t q = (uint64_t)n;
    /* first step whose top 2k cards include position n */
    uint64_t k = (q + 1) / 2;
    if (k == 0) k = 1;
    uint64_t m = 2 * k - 1;            /* becomes 2k+1 after the += 2 below */
    --k;
    for (;;) {
        ++k;
        m += 2;                        /* m = 2k + 1 */
        q <<= 1;
        if (q >= m) q -= m;
        if (q == 1) return (seq_t)k;
    }
}
