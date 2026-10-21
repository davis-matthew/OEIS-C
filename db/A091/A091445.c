/*
 * A091445  Concatenate binary vectors ordered first by length, then by the
 *          number of 1's and finally by a 'reverse' lexicographical ordering
 *          (read words from right to left).
 * https://oeis.org/A091445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9866, u16 n <= 9866, u32 n <= 9866, u64 n <= 9866,
 *            u128 n <= 9866, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A091445:   Concatenate binary vectors ordered first by length,
 *                  then by the number of 1's and finally by a reverse lexicographical
 *                  ordering (read words from right to left).  The function
 *                  computes the nth bit (0‑based) of this concatenation.
 *
 *  The algorithm enumerates, for each length L, all binary words of that
 *  length grouped by weight (number of 1’s) in ascending order.  Within a
 *  weight group the words are listed in ascending order of the reversed
 *  word (i.e. reverse lexicographic ascending).  Each word is emitted
 *  bit by bit, and the function returns the bit requested by index n.
 *
 *  Complexity:  O(n) per call for n up to about 10 000 (L never exceeds ~13).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t remaining = n;

    for (idx_t L = 1; ; ++L) {
        /* Total bits contributed by all words of length L */
        idx_t totalBits = L * ((idx_t)1 << L);
        if (remaining >= totalBits) {
            remaining -= totalBits;
            continue;
        }

        /* The desired bit lies within words of length L */
        for (idx_t w = 0; w <= L; ++w) {
            if (w == 0) {
                /* Only one word: all zeros */
                for (idx_t i = 0; i < L; ++i) {
                    if (remaining == 0) return (seq_t)0;
                    --remaining;
                }
            } else {
                /* Enumerate all reversed words with w bits set in ascending order. */
                idx_t rev = ((idx_t)1 << w) - 1;                 /* first combination   */
                idx_t last = (((idx_t)1 << w) - 1) << (L - w);   /* last combination    */
                for (;;) {
                    for (idx_t i = 0; i < L; ++i) {
                        if (remaining == 0) return (seq_t)((rev >> i) & 1);
                        --remaining;
                    }
                    if (rev == last) break;
                    /* Gosper's hack: next combination of w bits in L bits. */
                    idx_t c = rev & -rev;
                    idx_t r = rev + c;
                    rev = (((r ^ rev) >> 2) / c) | r;
                }
            }
        }
    }
}
