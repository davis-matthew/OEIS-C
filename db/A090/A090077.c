/*
 * A090077  In binary expansion of n: reduce contiguous blocks of 1's to 1.
 * https://oeis.org/A090077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = in the binary expansion of n, each maximal run of 1’s is
 *            replaced by a single 1, zeros are kept in place
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For idx_t = uint64_t, this implementation works for all builds
   (including the arbitrary‑precision seq_t). */
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    uint64_t m = (uint64_t)n;          /* "n" fits into uint64_t */
    int msb = 63 - __builtin_clzll(m); /* position of most significant 1 */

    seq_t res = 0;
    bool run_active = false;

    for (int i = msb; i >= 0; --i) {
        bool cur = (m >> i) & 1ULL;
        if (cur) {                    /* source bit is 1 */
            if (!run_active) {         /* start of a new run */
                res = (res << 1) | (seq_t)1;
                run_active = true;
            }
            /* else: continue the run – do nothing */
        } else {                      /* source bit is 0 */
            res <<= 1;                /* append 0 */
            run_active = false;
        }
    }
    return res;
}
