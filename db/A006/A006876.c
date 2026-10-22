/*
 * A006876  Mu-molecules in Mandelbrot set whose seeds have period n.
 * https://oeis.org/A006876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*l(n) - Sum_{d|n} phi(n/d)*l(d), where l(n) = Sum_{d|n}
 *            mu(n/d)*2^(d-1) (A000740)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int mobius_u64(uint64_t m) {
    int res = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            res = -res;
        }
    }
    if (m > 1) res = -res;
    return res;
}

static uint64_t totient_u64(uint64_t m) {
    uint64_t r = m, p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

static seq_t pow2_seq(uint64_t e) {
    seq_t result = 1, base = 2;
    while (e > 0) {
        if (e & 1) result = result * base;
        e >>= 1;
        if (e > 0) base = base * base;
    }
    return result;
}

/* l(m) = Sum_{d|m} mu(m/d) 2^(d-1) */
static seq_t l_val(uint64_t m) {
    seq_t pos = 0, neg = 0;
    uint64_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            uint64_t e = m / d;
            int mu = mobius_u64(m / d);
            if (mu > 0) pos += pow2_seq(d - 1);
            else if (mu < 0) neg += pow2_seq(d - 1);
            if (e != d) {
                mu = mobius_u64(d);
                if (mu > 0) pos += pow2_seq(e - 1);
                else if (mu < 0) neg += pow2_seq(e - 1);
            }
        }
    }
    return pos - neg;
}

seq_t a(idx_t n) {
    seq_t ln = l_val(n);
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            sum += (seq_t)totient_u64(e) * l_val(d);
            if (e != d) sum += (seq_t)totient_u64(d) * l_val(e);
        }
    }
    return (seq_t)2 * ln - sum;
}
