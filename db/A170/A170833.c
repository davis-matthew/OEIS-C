/*
 * A170833  a(n) = 4^(floor(n/2))+4^(floor(n/2)-1)-4^(floor((n-1)/3)).
 * https://oeis.org/A170833
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(⌊n/2⌋) + 4^(⌊n/2⌋-1) - 4^(⌊(n-1)/3⌋)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* values below offset are undefined */

    idx_t f  = n / 2;             /* ⌊n/2⌋ */
    idx_t f1 = f - 1;             /* ⌊n/2⌋‑1 (f ≥ 1 for n ≥ 2) */
    idx_t f2 = (n - 1) / 3;       /* ⌊(n‑1)/3⌋ */

    seq_t term1 = big_pow((seq_t)4, (seq_t)f);
    seq_t term2 = big_pow((seq_t)4, (seq_t)f1);
    seq_t term3 = big_pow((seq_t)4, (seq_t)f2);

    return term1 + term2 - term3;
}
