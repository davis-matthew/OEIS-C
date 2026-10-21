/*
 * A226224  The largest value of k in base n for which the sum of digits of k
 *          = sqrt(k).
 * https://oeis.org/A226224
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = largest k in base n such that sum_of_digits(k in base n)
 *            = sqrt(k)
 * Range:     u8 n <= 12, u16 n <= 156, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline seq_t digit_sum_base(seq_t value, seq_t base) {
    seq_t sum = 0;
    while (value) {
        sum += value % base;
        value /= base;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n < 2) return 0; /* undefined for n < 2 */
    /* maximum number of digits of k for given n (from OEIS comment) */
    idx_t max_digits = (n > 6) ? 3 : 4;
    seq_t Smax = (seq_t)max_digits * (((seq_t)n) - 1);

    for (seq_t S = Smax; S > 0; --S) {
        seq_t k = S * S;
        if (digit_sum_base(k, (seq_t)n) == S) {
            return k;
        }
    }
    return 0; /* no solution found, should not happen for n >= 2 */
}
