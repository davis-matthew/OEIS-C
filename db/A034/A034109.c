/*
 * A034109  Fractional part of square root of a(n) starts with 3: first term
 *          of runs.
 * https://oeis.org/A034109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil((n + 23/10)^2) = ((10*n + 23)^2 + 99) / 100
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)10 * n + (seq_t)23;
    return (t * t + (seq_t)99) / (seq_t)100;
}
