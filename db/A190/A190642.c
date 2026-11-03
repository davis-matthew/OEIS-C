/*
 * A190642  Numbers 3*n + 2 written in base 3.
 * https://oeis.org/A190642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10 * (decimal integer formed by the base‑3 digits of n) +
 *            2
 * Range:     u8 n <= 8, u16 n <= 80, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9986 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t base3_decimal(idx_t n)
{
    if (n == 0) return (seq_t)0;
    return base3_decimal(n / 3) * (seq_t)10 + (seq_t)(n % 3);
}
/* Recurrence: none; the decimal integer is computed recursively from the
 * base‑3 representation of n. */

seq_t a(idx_t n)
{
    return base3_decimal(n) * (seq_t)10 + (seq_t)2;
}
