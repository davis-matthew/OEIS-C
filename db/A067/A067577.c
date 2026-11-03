/*
 * A067577  a(n) = Product_{i=1..n} sigma(i) * Sum_{i=1..n} 1/sigma(i).
 * https://oeis.org/A067577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{i=1..n} σ(i)) * (∑_{i=1..n} 1/σ(i))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 417 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

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
    if (n == 0) return 0;                 /* undefined for n<1, harmless */
    seq_t product = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t s = sigma_of(i);
        product *= s;
    }
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t s = sigma_of(i);
        result += product / s;
    }
    return result;
}
