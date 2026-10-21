/*
 * A073213  Sum of two powers of 17.
 * https://oeis.org/A073213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 35, u64 n <= 135, u128 n <= 527,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A073213 – Sum of two powers of 17.
   For n ≥ 0 the n‑th term is given by
        e = ⌊(√(1+8n) – 1)/2⌋
        i = n – e(e+1)/2          (0 ≤ i ≤ e)
        a(n) = 17^i + 17^e
   (e is the largest integer with e(e+1)/2 ≤ n). */

#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Find e: the largest integer such that e*(e+1)/2 ≤ n. */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid;
        }
    }
    idx_t e = low;

    /* Compute i = n – e(e+1)/2. */
    __int128 prev128 = (__int128)e * (e + 1) / 2;
    idx_t   prev    = (idx_t)prev128;
    idx_t   i       = n - prev;

    /* Compute 17^i and 17^e. */
    seq_t pow_i = 1, pow_e = 1;
    for (idx_t k = 0; k < i; ++k) pow_i = mul_small(pow_i, 17);
    for (idx_t k = 0; k < e; ++k) pow_e = mul_small(pow_e, 17);

    return pow_i + pow_e;
}
