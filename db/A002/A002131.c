/*
 * A002131  Sum of divisors d of n such that n/d is odd.
 * https://oeis.org/A002131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, mult
 * Formula:   a(n) = sigma(n) - sigma(n/2) for even n; sigma(n) for odd n,
 *            where sigma(n) = sum of divisors
 * Range:     u8 n <= 164, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma(idx_t n) {
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            sum += (seq_t)i;
            if (i != n / i) {
                sum += (seq_t)(n / i);
            }
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    seq_t result = sigma(n);
    if (n % 2 == 0) {
        result -= sigma(n / 2);
    }
    return result;
}
