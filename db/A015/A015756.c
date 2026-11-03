/*
 * A015756  a(n) is the least multiple of n, k*n say, such that phi(k) |
 *          sigma(k).
 * https://oeis.org/A015756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least multiple m of n such that φ(m) | σ(m)
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 51 (2026-11-01)
 * Helpers:   totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* to use bool if needed */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient_seq.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* n is assumed to be >= 1; the offset is 1 */
    idx_t k = 1;
    for (;; ++k)
    {
        idx_t m = n * k;                     /* multiple of n */
        seq_t phi   = totient_seq((seq_t)m);    /* φ(m) */
        seq_t sigma = sigma_of((idx_t)m);    /* σ(m) */

        if (sigma % phi == 0)
            return (seq_t)m;                 /* a(n) found */
    }
}
