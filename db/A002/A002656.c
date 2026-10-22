/*
 * A002656  Expansion of (eta(q) * eta(q^7))^3 in powers of q.
 * https://oeis.org/A002656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   Multiplicative function from (eta(q)*eta(q^7))^3 expansion;
 *            a(7^e) = (-7)^e; a(p^e) = 0 if p ≡ 3,5,6 (mod 7) and e odd, else
 *            p^e; for p ≡ 1,2,4 (mod 7): a(p) = 2*(x^2 - 7*y^2) where p = x^2
 *            + 7*y^2, and a(p^e) = a(p)*a(p^(e-1)) - p^2*a(p^(e-2)) for e > 1
 * Range:     i8 n <= 57, i16 n <= 6553, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   compute_prime_power_i64, find_representation, isqrt_u64_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_prime_power_i64.h"
#include "../utils/find_representation.h"
#include "../utils/isqrt_u64_helper.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    int64_t result = 1;
    uint64_t temp = (uint64_t)n;
    
    for (uint64_t p = 2; p * p <= temp; p++) {
        uint64_t e = 0;
        while (temp % p == 0) {
            temp /= p;
            e++;
        }
        if (e > 0) {
            int64_t contrib = compute_prime_power_i64(p, e);
            result *= contrib;
        }
    }
    
    if (temp > 1) {
        int64_t contrib = compute_prime_power_i64(temp, 1);
        result *= contrib;
    }
    
    return (seq_t)result;
}
