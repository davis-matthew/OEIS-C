/*
 * A140548  Primes p such that neither p - 14 nor p + 14 is prime.
 * https://oeis.org/A140548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primes p such that neither p-14 nor p+14 is prime
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t found = 0;
    idx_t prime_idx = 1;
    seq_t ans = 0;
    while (found < n) {
        uint32_t p32 = get_prime(prime_idx);
        idx_t p = (idx_t)p32;
        bool ok = true;
        if (p > 14 && is_prime(p - 14))
            ok = false;
        if (ok && is_prime(p + 14))
            ok = false;
        if (ok) {
            found++;
            if (found == n)
                ans = (seq_t)p;
        }
        prime_idx++;
    }
    return ans;
}
