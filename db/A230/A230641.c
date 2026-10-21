/*
 * A230641  a(n) = n + (sum of digits in base-3 representation of n).
 * https://oeis.org/A230641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + sum of digits in base‑3 representation of n
 * Range:     u8 n <= 250, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        sum += tmp % 3;
        tmp /= 3;
    }
    return (seq_t)n + sum;
}
