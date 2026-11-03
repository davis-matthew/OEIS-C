/*
 * A068127  Triangular numbers with sum of digits = 3.
 * https://oeis.org/A068127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = T(2⋅10^(n-5)) = 10^(n-5)⋅(2⋅10^(n-5)+1) for n ≥ 7. For n
 *            = 1…6: 3, 21, 120, 210, 300, 10011
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 482 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* First six terms are handled explicitly. */
    static const seq_t first[6] = {3, 21, 120, 210, 300, 10011};

    if (n <= 6)
        return first[n - 1];

    /* For n ≥ 7, use the closed form with 10‑powers. */
    idx_t k = n - 5;                /* k ≥ 2 (since n ≥ 7) */
    seq_t pow10k = pow10(k);        /* 10^k */
    seq_t doubled = pow10k << 1;    /* 2⋅10^k */
    return pow10k * (doubled + 1);  /* 10^k ⋅ (2⋅10^k + 1) */
}
