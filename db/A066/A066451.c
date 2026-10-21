/*
 * A066451  a(n) is the number of integers k > 0 such that (n*k+1)/(k^2+1) is
 *          an integer.
 * https://oeis.org/A066451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force check k = 1..n, count if (n*k+1) % (k^2+1) == 0 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sk = (seq_t)k;
        seq_t denom = sk * sk + 1;
        seq_t numer = (seq_t)n * sk + 1;
        if (numer % denom == 0) {
            cnt += 1;
        }
    }
    return cnt;
}
