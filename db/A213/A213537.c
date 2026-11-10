/*
 * A213537  Record first differences of base sequence A213536 (a cousin prime
 *          recurrence sequence).
 * https://oeis.org/A213537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th record of the first differences of A213536, where
 *            A213536: b(1) = 14; b(k) = b(k-1) + gcd(k+5, b(k-1)) for even k,
 *            b(k) = b(k-1) + gcd(k+1, b(k-1)) for odd k > 1. Computed by
 *            direct iteration
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 18 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    static seq_t rec[128];
    static idx_t count = 0;
    static idx_t cur = 1;
    static seq_t prev = 14;
    static seq_t mx = 0;

    if (n == 0 || n > 128)
        return 0;

    while (count < n) {
        ++cur;
        seq_t g;
        if (cur & 1)
            g = gcd((seq_t)cur + 1, prev);
        else
            g = gcd((seq_t)cur + 5, prev);
        prev = prev + g;
        if (g > mx) {
            mx = g;
            rec[count++] = g;
        }
    }
    return rec[n - 1];
}
