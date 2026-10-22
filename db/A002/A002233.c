/*
 * A002233  a(1) = 1; for n > 1, a(n) = least positive prime primitive root of
 *          n-th prime.
 * https://oeis.org/A002233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1) = 1; for n > 1, a(n) = least positive prime primitive root
 *            of n-th prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_helper, nth_prime_helper, powmod_helper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_helper.h"
#include "../utils/nth_prime_helper.h"
#include "../utils/powmod_helper.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    
    uint64_t p = nth_prime_helper(n);
    uint64_t phi = p - 1;
    
    uint64_t factors[64];
    int nf = 0;
    
    uint64_t temp = phi;
    for (uint64_t d = 2; d * d <= temp; d++) {
        if (temp % d == 0) {
            factors[nf++] = d;
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) {
        factors[nf++] = temp;
    }
    
    for (uint64_t q = 2; q < p; q++) {
        if (!is_prime_helper(q)) continue;
        
        bool is_prim_root = true;
        for (int i = 0; i < nf; i++) {
            if (powmod_helper(q, phi / factors[i], p) == 1) {
                is_prim_root = false;
                break;
            }
        }
        
        if (is_prim_root) {
            return (seq_t)q;
        }
    }
    
    return 0;
}
