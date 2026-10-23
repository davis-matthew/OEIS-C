/*
 * A018524  Divisors of 570.
 * https://oeis.org/A018524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th divisor of 570 in ascending order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t target = (seq_t)570;
    idx_t count = 0;
    for (idx_t d = 1; d <= 570; ++d) {
        if (target % (seq_t)d == 0) {
            ++count;
            if (count == n) {
                return (seq_t)d;
            }
        }
    }
    return 0;
}
