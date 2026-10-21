/*
 * A211244  Order of 8 mod n-th prime: least k such that prime(n) divides
 *          8^k-1.
 * https://oeis.org/A211244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Order of 8 modulo the n‑th prime. * a(n) = ord_{p_n}(8) =
 *            ord_{p_n}(2) / gcd(3, ord_{p_n}(2)), * where ord_{p}(2) is the
 *            multiplicative order of 2 modulo a prime p
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* get_prime(idx_t k): k‑th prime (1‑based) */
#include "../utils/gcd.h"        /* gcd(seq_t a, seq_t b) */
#include "../utils/mod_pow.h"    /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */

seq_t a(idx_t n)
{
    /* Special case: for p = 2 (i.e., n = 1) the order is defined as 0. */
    if (n == 1)
        return 0;

    seq_t p = (seq_t)get_prime(n);                 /* n‑th prime */
    seq_t ord = p - 1;                             /* initial candidate for ord(2) */

    /* Factorise p-1.  Keep only unique prime factors. */
    uint64_t temp = (uint64_t)(ord);
    uint64_t primes[64];
    size_t pf_cnt = 0;

    for (uint64_t d = 2; d * d <= temp; ++d) {
        if (temp % d == 0) {
            primes[pf_cnt++] = d;
            while (temp % d == 0)
                temp /= d;
        }
    }
    if (temp > 1)
        primes[pf_cnt++] = temp;

    /* Reduce the order using its prime factors. */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t q = primes[i];          /* prime factor of p-1 */
        while ((ord % (seq_t)q) == 0) {
            uint64_t exp = (uint64_t)(ord / q);
            if (mod_pow(2, exp, p) == 1)
                ord /= q;
            else
                break;
        }
    }

    /* Compute final answer: ord / gcd(ord, 3). */
    seq_t g = gcd(ord, (seq_t)3);
    return ord / g;
}
