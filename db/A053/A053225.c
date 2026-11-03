/*
 * A053225  First differences of sigma(n) that are positive.
 * https://oeis.org/A053225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first positive difference of σ(n)
 * Range:     u8 n <= 54, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    if (n == 0) return 0;           /* invalid offset; return 0 */

    idx_t  count   = 0;
    idx_t  i       = 1;
    seq_t  sigma_prev = sigma_of(1); /* σ(1) = 1 */

    while (count < n) {
        seq_t sigma_next = sigma_of(i + 1);
        if (sigma_next > sigma_prev) {
            count++;
            if (count == n) return sigma_next - sigma_prev;
        }
        sigma_prev = sigma_next;
        i++;
    }

    return 0; /* unreachable, but satisfies all control paths */
}
