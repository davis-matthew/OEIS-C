/*
 * A129791  a(n) = ceiling(7^n/n).
 * https://oeis.org/A129791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(7^n / n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* the result is well‑defined only for n >= 1; otherwise return 0 */
    if (n == 0) return 0;

    /* If seq_t is 64‑bit or smaller we compute the power using a 128‑bit
       intermediate, which is large enough for all n whose a(n) fits
       in 64 bits.  For larger seq_t types the provided big_pow handles
       arbitrary precision. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 p = 1;
        for (idx_t i = 0; i < n; ++i)
            p *= 7;
        /* ceil = (p + n - 1) / n */
        unsigned __int128 numerator = p + (unsigned __int128)n - 1;
        seq_t result = (seq_t)(numerator / n);
        return result;
    } else {
        seq_t power = big_pow((seq_t)7, (seq_t)n);
        return (power + (seq_t)n - 1) / (seq_t)n;
    }
}
