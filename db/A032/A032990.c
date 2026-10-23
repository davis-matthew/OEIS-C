/*
 * A032990  If d,e are consecutive digits of n in base 8, then |d-e|>=4.
 * https://oeis.org/A032990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 836, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer whose octal representation has all consecutive
 * digits differing by at least 4. */
#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t count = 0;
    for (idx_t cand = 1; ; ++cand) {
        idx_t temp = cand;
        int last = temp % 8;
        temp /= 8;
        int ok = 1;
        while (temp > 0 && ok) {
            int d = temp % 8;
            int diff = last - d;
            if (diff < 0) diff = -diff;
            if (diff < 4) ok = 0;
            last = d;
            temp /= 8;
        }
        if (ok) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
    }
}
