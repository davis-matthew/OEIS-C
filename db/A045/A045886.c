/*
 * A045886  Erroneous version of A003462.
 * https://oeis.org/A045886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 1, a(2) = 4, a(3) = 13, a(4) = 121, for n ≥ 5: a(n) =
 *            121·3^(n‑4) + (3^(n‑4) – 1)/2
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 4;
    if (n == 3) return 13;
    if (n == 4) return 121;

    /* recurrence a(n) = 3·a(n‑1) + 1 for n ≥ 5 */
    /* closed‑form implementation using big_pow */
    idx_t e = n - 4;                   /* exponent for 3^(n‑4) */
    seq_t pow3 = big_pow((seq_t)3, (seq_t)e);
    seq_t term = pow3 * (seq_t)121 + (pow3 - (seq_t)1) / (seq_t)2;
    return term;
}
