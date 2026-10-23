/*
 * A143274  a(n) = n * A006218(n).
 * https://oeis.org/A143274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sum_{i=1}^n floor(n/i)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;
    idx_t i = 1;

    while (i <= n) {
        idx_t q = n / i;          /* floor(n/i) */
        idx_t last = n / q;       /* largest i with same floor value */
        seq_t count = (seq_t)(last - i + 1);
        sum += count * (seq_t)q;
        i = last + 1;
    }

    return (seq_t)n * sum;
}
