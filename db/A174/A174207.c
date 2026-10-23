/*
 * A174207  Depleted stream of the natural numbers written in base 2: delete
 *          even occurrences of digit 0 and odd occurrences of digit 1.
 * https://oeis.org/A174207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Depleted binary stream: keep 0 on odd occurrences, keep 1 on even occurrences. */

seq_t a(idx_t n) {
    idx_t count0 = 0, count1 = 0;   /* occurrence counters for 0 and 1 */
    idx_t kept  = 0;                 /* number of kept digits seen so far */
    seq_t result = 0;                /* value to return */

    for (idx_t num = 0; ; ++num) {
        /* Special case for number 0: its binary representation is the single digit 0 */
        if (num == 0) {
            ++count0;                     /* first 0 */
            if (count0 & 1) {              /* keep on odd occurrence */
                if (kept == n) { result = 0; goto done; }
                ++kept;
            }
            continue;
        }

        /* Process the bits of 'num' from most‑significant to least‑significant */
        unsigned long long u = (unsigned long long)num;
        int msb = 0;                       /* index of most significant bit */
        unsigned long long t = u;
        while (t >>= 1) ++msb;             /* compute msb safely for uint64_t */

        for (int pos = msb; pos >= 0; --pos) {
            int bit = (u >> pos) & 1;
            if (bit == 0) {
                ++count0;
                if (count0 & 1) {          /* keep 0 on odd occurrence */
                    if (kept == n) { result = 0; goto done; }
                    ++kept;
                }
            } else {
                ++count1;
                if ((count1 & 1) == 0) {   /* keep 1 on even occurrence */
                    if (kept == n) { result = 1; goto done; }
                    ++kept;
                }
            }
        }
    }

done:
    return result;
}
