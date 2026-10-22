/*
 * A006507  a(n+1) = a(n) + sum of digits of a(n), with a(1)=7.
 * https://oeis.org/A006507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = a(n-1) + sum_of_digits(a(n-1)), a(1) = 7
 * Range:     u8 n <= 25, u16 n <= 3391, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t digit_sum(seq_t x) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (n == 1) return 7;
    
    seq_t val = 7;
    for (idx_t i = 2; i <= n; i++) {
        val = val + digit_sum(val);
    }
    return val;
}
