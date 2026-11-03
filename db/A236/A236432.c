/*
 * A236432  a(n) = (2*n-1)*210; numbers which are 210 times an odd number.
 * https://oeis.org/A236432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*210
 * Range:     u8 n <= 1, u16 n <= 156, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 57 terms through n = 9822 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (seq_t)210 * ((seq_t)2 * sn - (seq_t)1);
}
