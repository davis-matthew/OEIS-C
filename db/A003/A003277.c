/*
 * A003277  Cyclic numbers: k such that k and phi(k) are relatively prime;
 *          also k such that there is just one group of order k, i.e.,
 *          A000001(k) = 1.
 * https://oeis.org/A003277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   n is squarefree product of distinct primes p where no p_i-1 is
 *            divisible by any p_j (i ≠ j)
 * Range:     u8 n <= 87, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int get_prime_factors(idx_t n, uint64_t *factors) {
    int count = 0;
    
    if (n % 2 == 0) {
        factors[count++] = 2;
        n /= 2;
        if (n % 2 == 0) return -1;
    }
    
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) {
            factors[count++] = p;
            n /= p;
            if (n % p == 0) return -1;
        }
    }
    
    if (n > 1) {
        factors[count++] = n;
    }
    
    return count;
}

static bool is_cyclic(idx_t m) {
    if (m == 1) return true;
    
    uint64_t factors[64];
    int nfactors = get_prime_factors(m, factors);
    
    if (nfactors <= 0) return false;
    
    for (int i = 0; i < nfactors; i++) {
        idx_t pi_minus_1 = factors[i] - 1;
        for (int j = 0; j < nfactors; j++) {
            if (i != j && pi_minus_1 % factors[j] == 0) {
                return false;
            }
        }
    }
    
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (idx_t m = 1; count < n; m++) {
        if (is_cyclic(m)) {
            count++;
            if (count == n) {
                return (seq_t)m;
            }
        }
    }
    return (seq_t)0;
}
