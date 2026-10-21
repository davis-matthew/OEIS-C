/*
 * A080221  n is Harshad (divisible by the sum of its digits) in a(n) bases
 *          from 1 to n.
 * https://oeis.org/A080221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{ b : 1 ≤ b ≤ n and n is a Harshad number in base b }|
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    seq_t result = 1;              /* base 1 (unary) always works */
    seq_t n_seq = (seq_t)n;        /* work with seq_t for all arithmetic */

    for (idx_t base = 2; base <= n; ++base) {
        seq_t b = (seq_t)base;
        seq_t sum = 0;
        seq_t cur = n_seq;

        /* compute digit sum of n in base b */
        while (cur) {
            sum += cur % b;
            cur /= b;
        }

        /* sum is always > 0 for base >= 2 */
        if (n_seq % sum == 0) {
            result += 1;
        }
    }
    return result;
}
