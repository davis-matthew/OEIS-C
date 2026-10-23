/*
 * A191690  a(n) = n^n-n^(n-1)-n^(n-2)-...-n^2-n-1.
 * https://oeis.org/A191690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n - n^(n-1) - ... - n^2 - n - 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;

    /* Handle 64‑bit builds with 128‑bit intermediates */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 pow = 1;
        for (idx_t i = 0; i < n; ++i) pow *= n;

        unsigned __int128 numerator = pow * (n - 2) + 1;
        unsigned __int128 denom    = n - 1;
        unsigned __int128 result   = numerator / denom;
        return (seq_t)result;
    }
    /* General case for 128‑bit or arbitrary precision types */
    seq_t pow = 1;
    for (idx_t i = 0; i < n; ++i) pow *= (seq_t)n;

    seq_t numerator = pow * (seq_t)(n - 2) + 1;
    seq_t denom     = (seq_t)(n - 1);
    return numerator / denom;
}
