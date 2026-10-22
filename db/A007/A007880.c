/*
 * A007880  Westminster chimes at 15-minute intervals (1).
 * https://oeis.org/A007880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, 2, 3 for n mod 4 = 0, 1, 2; a(n) = 5 + floor(n/4) mod
 *            12 for n mod 4 = 3 (period 48)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 4;

    if (r == 0) return (seq_t)1;
    if (r == 1) return (seq_t)2;
    if (r == 2) return (seq_t)3;
    return (seq_t)5 + (seq_t)((n / 4) % 12);
}
