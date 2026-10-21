/*
 * A099649  Solutions to A099648(k) > k; i.e., numbers such that the largest
 *          term in the iteration of the A003132() function strictly exceeds
 *          the initial value.
 * https://oeis.org/A099649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 130, u16 n <= 130, u32 n <= 130, u64 n <= 130,
 *            u128 n <= 130, big any n
 * Helpers:   digit_square_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A099649 – Numbers n for which the maximum attained while
   iterating the digit‑square‑sum map exceeds n.
   The sequence is finite; the last term is 144.
   Computation is done by generating all candidates up to 144. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_square_sum.h"

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t seq[130];
    static idx_t count = 0;

    if (!initialized) {
        for (idx_t m = 2; m <= 144; ++m) {
            uint64_t start = (uint64_t)m;
            uint64_t max_seen = start;
            uint64_t cur = start;
            bool visited[256] = {};            /* sufficient for all intermediate values */

            visited[(int)cur] = true;
            while (1) {
                uint64_t nxt = digit_square_sum(cur);
                if (nxt > max_seen) {
                    max_seen = nxt;
                    if (max_seen > start) {      /* property satisfied */
                        seq[count++] = (seq_t)start;
                        break;
                    }
                }
                if (nxt == 1 || (nxt < 256 && visited[(int)nxt])) {
                    break;                       /* cycle reached without exceeding start */
                }
                if (nxt < 256) visited[(int)nxt] = true;
                cur = nxt;
            }
        }
        initialized = true;
    }

    if (n >= 1 && n <= count)
        return seq[n - 1];
    return 0;   /* out of range – no such term */
}
