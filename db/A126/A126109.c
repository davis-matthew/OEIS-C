/*
 * A126109  a(n) = (5*10^n + 1)/3.
 * https://oeis.org/A126109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = (5*10^n + 1)/3
   Equivalent recurrence used: a(0) = 2,  a(n) = 10*a(n-1) - 3   for n ≥ 1
   This recurrence avoids large intermediate products and works for
   all supported seq_t types, whether 64‑bit or arbitrary precision. */
seq_t a(idx_t n)
{
    seq_t val = 2;
    for (idx_t i = 1; i <= n; ++i)
        val = val * 10 - 3;
    return val;
}
