/*
 * A236455  Number of zero digits in all representations of n in bases
 *          2,3,...,10.
 * https://oeis.org/A236455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{b=2}^{10} (number of digits equal to 0 in the
 *            representation of n in base b)
 * Range:     u8 n <= 270, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t b = 2; b <= 10; ++b) {
        seq_t base = (seq_t)b;
        seq_t m = (seq_t)n;
        while (m > 0) {
            seq_t digit = m % base;
            if (digit == 0)
                sum++;
            m /= base;
        }
    }
    return sum;
}
