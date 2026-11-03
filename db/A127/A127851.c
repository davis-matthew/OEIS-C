/*
 * A127851  a(n) has n 1's followed by C(n-1,2) 0's.
 * https://oeis.org/A127851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^C(n,2) * (10^n-1) / (9*10^(n-1))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 9 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(n) = n 1's followed by C(n-1,2) 0's for n ≥ 1 */

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
    if (n == 0)
        return (seq_t)0;

    /* 10^n */
    seq_t pow10n = pow10(n);

    /* (10^n - 1) / 9  →  n ones */
    seq_t ones = (pow10n - (seq_t)1) / 9;

    /* number of trailing zeros: C(n-1,2) = (n-1)(n-2)/2 */
    idx_t zeros = 0;
    if (n >= 2)
        zeros = ((n - 1) * (n - 2)) / 2;

    /* multiply by 10^zeros to shift the zeros to the right */
    seq_t pow10z = pow10(zeros);

    return ones * pow10z;
}
