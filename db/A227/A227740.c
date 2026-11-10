/*
 * A227740  Integers from 0 to A037834(n) followed by integers from 0 to
 *          A037834(n+1) and so on.
 * https://oeis.org/A227740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - (1 + A173318(A227737(n)-1)); computed by a linear
 *            scan over blocks of length A005811(k)
 * Range:     u8 n <= 2272, u16 n <= 2272, u32 n <= 2272, u64 n <= 2272,
 *            u128 n <= 2272, big any n
 * Validated: standard, pass, 105 terms through n = 2268 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    idx_t prev = 0; /* sum of runs(1..k-1) */
    for (idx_t k = 1; ; ++k) {
        idx_t c = (idx_t)__builtin_popcountll((unsigned long long)(k ^ (k >> 1)));
        if (prev + c >= n) return (seq_t)(n - 1 - prev);
        prev += c;
    }
}
