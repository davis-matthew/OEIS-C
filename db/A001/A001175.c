/*
 * A001175  Pisano periods (or Pisano numbers): period of Fibonacci numbers
 *          mod n.
 * https://oeis.org/A001175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = lcm(a(p1^e1), ..., a(pk^ek)) for n = p1^e1...pk^ek, with
 *            a(p^e) = p^(e-1)*a(p) (Wall); a(p) is found by iterating
 *            Fibonacci numbers mod p until the pair (0,1) recurs
 * Range:     u8 n <= 49, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pis_gcd, pis_lcm, pisano_prime, pisano_prime_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pis_gcd.h"
#include "../utils/pis_lcm.h"
#include "../utils/pisano_prime.h"
#include "../utils/pisano_prime_power.h"

seq_t a(idx_t n) {
    uint64_t temp = n, result = 1, p;
    if (n <= 1) return (seq_t)1;
    for (p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) { temp /= p; e++; }
            result = pis_lcm(result, pisano_prime_power(p, e));
        }
    }
    if (temp > 1) result = pis_lcm(result, pisano_prime_power(temp, 1));
    return (seq_t)result;
}
