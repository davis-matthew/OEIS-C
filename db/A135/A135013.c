/*
 * A135013  Partial sums of A000265.
 * https://oeis.org/A135013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k≥1} (round(n/2^k))^2
 * Range:     u8 n <= 26, u16 n <= 442, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* maximum shift value for which 1 << shift is well‑defined */
    size_t max_shift = sizeof(seq_t) * 8 - 1;

    for (idx_t k = 1; k <= max_shift; ++k)
    {
        /* round(n/2^k) = (n + 2^{k-1}) >> k */
        seq_t shifted = ((seq_t)n + ((seq_t)1 << (k - 1))) >> k;
        if (!shifted)
            break;
        sum += shifted * shifted;
    }

    return sum;
}
