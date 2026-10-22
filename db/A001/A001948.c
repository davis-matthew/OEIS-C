/*
 * A001948  These numbers when multiplied by all powers of 4 give the numbers
 *          that are not the sums of 4 distinct squares.
 * https://oeis.org/A001948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   search: a(n) = n-th positive m with m mod 4 != 0 such that m is
 *            not a^2+b^2+c^2+d^2 with 0 <= a < b < c < d
 * Range:     u8 n <= 31, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   a001948_is_s4d
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a001948_is_s4d.h"

seq_t a(idx_t n)
{
    static long found[64];
    static int nfound = -1;
    long m;
    if (nfound < 0) {
        nfound = 0;
        for (m = 1; m <= 2000 && nfound < 64; m++) {
            if (m % 4 == 0) continue;
            if (!a001948_is_s4d(m)) found[nfound++] = m;
        }
    }
    if (n < 1 || n > (idx_t)nfound) return (seq_t)0;
    return (seq_t)found[n - 1];
}
