/*
 * A007624  Numbers m such that the product of proper divisors of m = m^k,
 *          k>1.
 * https://oeis.org/A007624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Numbers m such that d(m) > 4 and d(m) is even, where d(m) is the
 *            number of divisors of m
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static uint64_t count_divisors(uint64_t m) {
    uint64_t count = 0;
    for (uint64_t i = 1; i * i <= m; i++) {
        if (m % i == 0) {
            count++;
            if (i * i != m) {
                count++;
            }
        }
    }
    return count;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t m = 11;
    
    while (count < n) {
        m++;
        uint64_t divisor_count = count_divisors((uint64_t)m);
        if (divisor_count > 4 && divisor_count % 2 == 0) {
            count++;
        }
    }
    
    return m;
}
