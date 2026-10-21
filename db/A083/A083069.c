/*
 * A083069  Main diagonal of number array A083064.
 * https://oeis.org/A083069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n * (n+2)^n + 1) / (n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute (n+2)^n.  The helper big_pow handles large values when
       seq_t is an arbitrary‑precision type and avoids intermediate
       overflow for the 64‑bit build. */
    seq_t pow_val = big_pow((seq_t)(n + 2), (seq_t)n);

    /* Divide first to keep values small enough for 64‑bit arithmetic.
       (pow_val - 1) is always divisible by (n+1). */
    seq_t t = (pow_val - 1) / ((seq_t)(n + 1));

    /* Final result.  For n ≤ 15 the multiplication fits in 64 bits
       and for arbitrary‑precision builds it is exact. */
    return 1 + (seq_t)n * t;
}
