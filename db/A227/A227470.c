/*
 * A227470  Least k such that n divides sigma(n*k).
 * https://oeis.org/A227470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n divides sigma(n*k)
 * Range:     u8 n <= 10, u16 n <= 246, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    seq_t n_seq = (seq_t)n;

    for (idx_t k = 1; ; ++k) {
        idx_t m = n * k;           /* product fits in 64‑bit for n ≤ 10⁴ */
        seq_t sig = sigma_of(m);
        if ((sig % n_seq) == 0) {
            return (seq_t)k;       /* minimal k found */
        }
        /* the theorem guarantees eventual success; loop will terminate */
    }
}
