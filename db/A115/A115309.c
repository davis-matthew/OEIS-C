/*
 * A115309  a(n) = if n<9 then n else 9*a(floor(n/9)) + 8 - n mod 9.
 * https://oeis.org/A115309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n < 9 then n else 9 * a(floor(n/9)) + (8 - n % 9)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 9)
        return (seq_t)n;

    seq_t sub = a(n / 9);
    seq_t d   = (seq_t)(n % 9);
    return (seq_t)9 * sub + (seq_t)(8 - d);
}
