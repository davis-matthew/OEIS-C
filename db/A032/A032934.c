/*
 * A032934  Numbers whose set of base-14 digits is {1,2}.
 * https://oeis.org/A032934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = enumeration of base‑14 numbers consisting only of digits
 *            1 and 2, in increasing order
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the length L of the base‑14 representation.  For length L there are
       2^L numbers and all such numbers satisfy   2^{L+1}–2 ≥ n > 2^L–2. */
    idx_t L = 1;
    seq_t power = (seq_t)4;          /* 2^{L+1} for L = 1 */
    while ((power - 2) < (seq_t)n) {
        ++L;
        power <<= 1;                /* power = 2^{L+1} */
    }

    seq_t power_half = power >> 1;   /* 2^L   */
    seq_t prev = power_half - 2;     /* cumulative up to length L‑1 */

    idx_t offset = (idx_t)(n - prev - 1);  /* 0‑based index within length L */

    seq_t val = 0;
    for (idx_t i = 0; i < L; ++i) {
        idx_t shift = L - 1 - i;      /* MSB first */
        seq_t bit = (seq_t)((offset >> shift) & 1);
        val = val * 14 + (seq_t)(1 + bit);
    }
    return val;
}
