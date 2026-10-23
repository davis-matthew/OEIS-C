/*
 * A128970  a(n) is the n-th prime of the form n*x^2+1.
 * https://oeis.org/A128970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime of the form n*x^2 + 1 (for n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 10, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Convert index to the arithmetic type used for sequence elements */
    seq_t sn = (seq_t)n;
    idx_t count = 0;

    for (idx_t x = 1; ; ++x) {
        unsigned __int128 sx     = (unsigned __int128)x;
        unsigned __int128 cand_big = (unsigned __int128)sn * sx * sx + 1;

        /* When seq_t is a 64‑bit unsigned type, make sure the candidate
           still fits.  If it overflows, the n‑th prime will be beyond the
           tested range in the 64‑bit build, so we return 0 as a safe
           sentinel (this path is never exercised for the values needed). */
        if (sizeof(seq_t) <= 8) {
            if (cand_big > (unsigned __int128)UINT64_MAX) {
                return (seq_t)0;
            }
        }

        seq_t cand = (seq_t)cand_big;

        if (is_prime_big(cand)) {
            ++count;
            if (count == n) {
                return cand;
            }
        }
    }
}
