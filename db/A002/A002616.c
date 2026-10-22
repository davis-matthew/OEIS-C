/*
 * A002616  Reduced totient function (divided by 2).
 * https://oeis.org/A002616
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002322(n)/2, where A002322 is the Carmichael lambda
 *            function
 * Range:     u8 n <= 520, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t gcd_seq(seq_t x, seq_t y) {
    while (y) {
        seq_t t = y;
        y = x % y;
        x = t;
    }
    return x;
}

static seq_t lcm_seq(seq_t x, seq_t y) {
    return x / gcd_seq(x, y) * y;
}

static seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0) {
        if (exp & 1) result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

static seq_t lambda_prime_power(idx_t p, idx_t k) {
    if (p == 2) {
        if (k == 1) return 1;
        if (k == 2) return 2;
        return power_seq(2, k - 2);
    } else {
        return power_seq((seq_t)p, k - 1) * (seq_t)(p - 1);
    }
}

static seq_t carmichael_lambda(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    idx_t un = n;
    
    for (idx_t p = 2; p * p <= un; p++) {
        if (un % p == 0) {
            idx_t k = 0;
            while (un % p == 0) {
                k++;
                un /= p;
            }
            seq_t lp = lambda_prime_power(p, k);
            result = lcm_seq(result, lp);
        }
    }
    
    if (un > 1) {
        seq_t lp = lambda_prime_power(un, 1);
        result = lcm_seq(result, lp);
    }
    
    return result;
}

seq_t a(idx_t n) {
    seq_t lam = carmichael_lambda(n);
    return lam / 2;
}
