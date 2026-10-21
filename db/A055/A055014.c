/*
 * A055014  Sum of 5th powers of digits of n.
 * https://oeis.org/A055014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = Sum of the fifth powers of the decimal digits of n
 *            Recurrence: a(10n + k) = a(n) + k^5 (0 <= k < 10), a(0) = 0
 * Range:     u8 n <= 3, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t pow5[10] = {
        (seq_t)0,
        (seq_t)1,
        (seq_t)32,
        (seq_t)243,
        (seq_t)1024,
        (seq_t)3125,
        (seq_t)7776,
        (seq_t)16807,
        (seq_t)32768,
        (seq_t)59049
    };
    seq_t sum = 0;
    idx_t x = n;
    while (x) {
        idx_t digit = x % 10;
        sum += pow5[digit];
        x /= 10;
    }
    return sum;
}
