/*
 * A002444  Denominator in Feinler's formula for unsigned Bernoulli number
 *          |B_{2n}|.
 * https://oeis.org/A002444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (Prod_i (p_i)^V(n,i)) / gcd(Prod_i (p_i)^V(n,i), (2n)!)
 *            where V(n,i) = floor(2n/(prime(i)-1)) and product is over primes
 *            where V(n,i) > 0. In reduced form, a(n) is the numerator
 *            (equivalently, the denominator of unsigned Bernoulli |B_{2n}|)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 35, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   vp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/vp.h"

static bool is_prime(uint64_t n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    
    uint64_t two_n = 2 * (uint64_t)n;
    seq_t result = 1;
    
    for (uint64_t p = 2; p <= two_n + 1; p++) {
        if (!is_prime(p)) continue;
        
        uint64_t exp_in_prod = two_n / (p - 1);
        uint64_t exp_in_factorial = vp(two_n, p);
        
        if (exp_in_prod > exp_in_factorial) {
            uint64_t exp = exp_in_prod - exp_in_factorial;
            for (uint64_t i = 0; i < exp; i++) {
                result *= (seq_t)p;
            }
        }
    }
    
    return result;
}
