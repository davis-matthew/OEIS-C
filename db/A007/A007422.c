/*
 * A007422  Multiplicatively perfect numbers j: product of divisors of j is
 *          j^2.
 * https://oeis.org/A007422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers with exactly 4 divisors, plus 1 (2-multiplicatively perfect) */

static idx_t count_divisors(seq_t n) {
    idx_t count = 0;
    for (seq_t i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            count++;
            if (i != n / i) count++;
        }
    }
    return count;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    idx_t count = 1;
    seq_t j = 1;
    
    while (count < n) {
        j++;
        if (count_divisors(j) == 4) {
            count++;
        }
    }
    
    return j;
}
