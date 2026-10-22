/*
 * A004085  Sum of digits of Euler totient function of n.
 * https://oeis.org/A004085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A007953(A000010(n))
 * Range:     u8 n <= 354, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t euler_phi(seq_t n) {
    seq_t result = n;
    seq_t temp = n;
    seq_t p = 2;
    
    while (p <= temp / p) {
        if (temp % p == 0) {
            while (temp % p == 0) {
                temp /= p;
            }
            result = result / p * (p - 1);
        }
        p++;
    }
    
    if (temp > 1) {
        result = result / temp * (temp - 1);
    }
    
    return result;
}

static seq_t digit_sum(seq_t n) {
    seq_t sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

seq_t a(idx_t n) {
    return digit_sum(euler_phi((seq_t)n));
}
