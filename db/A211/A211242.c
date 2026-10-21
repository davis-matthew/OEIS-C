/*
 * A211242  Order of 6 mod n-th prime: least k such that prime(n) divides
 *          6^k-1.
 * https://oeis.org/A211242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A211242: order of 6 modulo the n-th prime.
   a(n) = 0 for the first two primes (2 and 3, where 6≡0),
   otherwise the smallest k>0 such that 6^k ≡ 1 (mod prime(n)). */
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);
    if (p == 2 || p == 3) return (seq_t)0;

    seq_t phi = (seq_t)(p - 1);
    uint64_t tmp = (uint64_t)p - 1;

    unsigned facs[64];
    size_t fcnt = 0;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            facs[fcnt++] = (unsigned)d;
            while (tmp % d == 0) tmp /= d;
        }
    }
    if (tmp > 1) facs[fcnt++] = (unsigned)tmp;

    seq_t ans = phi;
    for (size_t i = 0; i < fcnt; ++i) {
        unsigned f = facs[i];
        while (ans % f == 0) {
            seq_t test = mod_pow((seq_t)6, (uint64_t)(ans / f), (seq_t)p);
            if (test != 1) break;
            ans /= f;
        }
    }
    return ans;
}
