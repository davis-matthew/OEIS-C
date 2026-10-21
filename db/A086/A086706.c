/*
 * A086706  Number of Niven numbers less than or equal to n.
 * https://oeis.org/A086706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = #{ m ≤ n : m % digit_sum(m) == 0 } (number of Niven numbers ≤ n) */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;
        unsigned sum = 0;
        while (x > 0) {
            sum += x % 10;
            x /= 10;
        }
        if (i % sum == 0) {
            ++count;
        }
    }
    return count;
}
