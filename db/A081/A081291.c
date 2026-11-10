/*
 * A081291  Complement of A072795.
 * https://oeis.org/A081291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(n) = n + A014137(A081288(n)-1), i.e. n plus the sum of
 *            Catalan numbers C_0..C_{i-1}, where i is the least index with
 *            C_i > n
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 63 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t sum = 0;
    seq_t cat = 1;
    seq_t k = 0;
    while (cat <= nn) {
        sum += cat;
        cat = cat * (4 * k + 2) / (k + 2);
        ++k;
    }
    return nn + sum;
}
