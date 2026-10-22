/*
 * A007955  Product of divisors of n.
 * https://oeis.org/A007955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = n^(tau(n)/2), where tau(n) is the number of divisors of n
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

static uint64_t isqrt(uint64_t n) {
    if (n == 0) return 0;
    uint64_t x = n;
    uint64_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + n / x) / 2;
    }
    return x;
}

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    
    // Count divisors of n
    idx_t count = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            count++;
            if (i * i != n) {
                count++;
            }
        }
    }
    
    // Compute n^(count/2)
    if (count % 2 == 0) {
        // count is even, compute n^(count/2)
        idx_t exp = count / 2;
        return power(n_seq, exp);
    } else {
        // count is odd, n must be a perfect square
        // compute sqrt(n)^count
        uint64_t sqrt_n = isqrt(n);
        return power((seq_t)sqrt_n, count);
    }
}
