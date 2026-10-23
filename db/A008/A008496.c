/*
 * A008496  a(n) = floor(n/5)*floor((n+1)/5)*floor((n+2)/5).
 * https://oeis.org/A008496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/5)*floor((n+1)/5)*floor((n+2)/5)
 * Range:     u8 n <= 33, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t div5 = n / 5;
    idx_t mod5 = n % 5;
    seq_t t1 = (seq_t)div5;
    seq_t t2 = (seq_t)(div5 + (mod5 == 4 ? 1 : 0));
    seq_t t3 = (seq_t)(div5 + (mod5 >= 3 ? 1 : 0));
    return t1 * t2 * t3;
}
