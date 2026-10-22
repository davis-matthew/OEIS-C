/*
 * A003624  Duffinian numbers: composite numbers k relatively prime to
 *          sigma(k).
 * https://oeis.org/A003624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Composite numbers k relatively prime to sigma(k), where sigma(k)
 *            is sum of divisors
 * Range:     u8 n <= 34, u16 n <= 5301, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool is_prime(seq_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    
    seq_t i = 3;
    while (i * i <= n) {
        if (n % i == 0) return false;
        i += 2;
    }
    return true;
}

static seq_t compute_sigma(seq_t n) {
    seq_t sigma = 0;
    seq_t i = 1;
    
    while (i * i <= n) {
        if (n % i == 0) {
            sigma += i;
            if (i != n / i) {
                sigma += n / i;
            }
        }
        i++;
    }
    return sigma;
}

static seq_t gcd(seq_t a, seq_t b) {
    while (b) {
        seq_t temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

seq_t a(idx_t n) {
    seq_t count = 0;
    seq_t k = 4;
    
    while (count < n) {
        if (!is_prime(k)) {
            seq_t sig = compute_sigma(k);
            if (gcd(k, sig) == 1) {
                count++;
                if (count == n) {
                    return k;
                }
            }
        }
        k++;
    }
    
    return k;
}
