/*
 * A039386  Numbers whose base-9 representation has the same nonzero number of
 *          4's and 8's.
 * https://oeis.org/A039386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers whose base‑9 representation has equal nonzero numbers of 4's and 8's. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against 0 */
    seq_t candidate = (seq_t)1;
    idx_t  found    = 0;

    while (1) {
        seq_t temp = candidate;
        int    cnt4 = 0;
        int    cnt8 = 0;

        while (temp > 0) {
            int digit = (int)(temp % (seq_t)9);
            if (digit == 4) ++cnt4;
            else if (digit == 8) ++cnt8;
            temp = temp / (seq_t)9;
        }

        if (cnt4 == cnt8 && cnt4 > 0) {
            ++found;
            if (found == n) return candidate;
        }

        candidate = candidate + (seq_t)1;
    }
}
