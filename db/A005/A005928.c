/*
 * A005928  G.f.: s(1)^3/s(3), where s(k) = eta(q^k) and eta(q) is Dedekind's
 *          function, cf. A010815.
 * https://oeis.org/A005928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -3 * A123477(n) for n > 0, a(0) = 1, where A123477 is
 *            multiplicative with: A123477(p^e) = -2 if p = 3 and e > 0
 *            A123477(p^e) = e+1 if p ≡ 1 (mod 6) A123477(p^e) = (1 +
 *            (-1)^e)/2 if p ≡ 2, 5 (mod 6)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9322 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t compute_A123477(idx_t n) {
    if (n == 1) return 1;
    
    seq_t result = 1;
    
    // Factor out 2
    if (n % 2 == 0) {
        idx_t e = 0;
        while (n % 2 == 0) {
            e++;
            n /= 2;
        }
        if (e % 2 == 1) {
            return 0;
        }
    }
    
    // Factor out 3
    if (n % 3 == 0) {
        while (n % 3 == 0) {
            n /= 3;
        }
        result *= -2;
    }
    
    // Factor out primes p >= 5
    for (idx_t p = 5; p * p <= n; p += 2) {
        if (n % p == 0) {
            idx_t e = 0;
            while (n % p == 0) {
                e++;
                n /= p;
            }
            if (p % 6 == 5) {
                if (e % 2 == 1) {
                    return 0;
                }
            } else {
                // p % 6 == 1
                result *= (seq_t)(e + 1);
            }
        }
    }
    
    // Handle remaining prime (if n > 1)
    if (n > 1) {
        if (n % 6 == 5) {
            return 0;
        } else {
            // n % 6 == 1
            result *= 2;
        }
    }
    
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return -3 * compute_A123477(n);
}
