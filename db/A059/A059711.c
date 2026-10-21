/*
 * A059711  Smallest base in which n is a repdigit.
 * https://oeis.org/A059711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest base b ≥ 2 such that the base‑b representation
 *            of n consists of all equal digits (a repdigit)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: 0 is represented as "0" in any base, so the smallest base is 2. */
    if (n == 0) return (seq_t)2;

    seq_t N = (seq_t)n;          /* Work in the seq_t domain for safety. */
    idx_t limit = n + 1;         /* base n+1 yields a single digit representation. */

    for (idx_t bIdx = 2; bIdx <= limit; ++bIdx) {
        seq_t b = (seq_t)bIdx;
        seq_t cur = N;
        seq_t digit = cur % b;   /* First (least significant) digit. */
        bool repdigit = true;

        while (cur > 0) {
            if (cur % b != digit) {
                repdigit = false;
                break;
            }
            cur /= b;
        }

        if (repdigit) return b;
    }

    /* The loop always succeeds because base (n+1) is a trivial solution. */
    return (seq_t)(n + 1);
}
