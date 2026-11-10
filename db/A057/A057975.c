/*
 * A057975  Record depths reached by card 1 in the shuffle in A035485.
 * https://oeis.org/A057975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   simulate Guy's shuffle for card 1 (start at position p=1); at
 *            shuffle k, p -> 2p if p<=k, p -> 2(p-k)-1 if k<p<=2k, else
 *            unchanged; a(n) = the (n+1)-th strict record of p
 * Range:     u8 n <= 18, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 51 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t p = 1;
    seq_t best = 1;
    idx_t count = 0;
    for (idx_t k = 1;; ++k) {
        seq_t kk = (seq_t)k;
        if (p <= kk) {
            p = p + p;
        } else if (p <= kk + kk) {
            p = (p - kk) * 2 - 1;
        }
        if (p > best) {
            best = p;
            if (count == n) return p;
            ++count;
        }
    }
}
