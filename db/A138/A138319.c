/*
 * A138319  Number of ways in which 2n can be represented as p+g, where p is a
 *          prime and g is a prime primitive root of p.
 * https://oeis.org/A138319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations 2n = p + g with * p prime, g
 *            prime primitive root mod p. The code follows the * definition
 *            directly and uses the provided utilities for primality, *
 *            modular exponentiation, prime sieving and smallest‑prime‑factor
 *            tables
 * Range:     u8 n <= 385, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, build_spf, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"

static bool initialized = false;
static idx_t limit = 20000;                /* 2 * 10000, covers all n in the limits */
static size_t prime_cnt = 0;
static uint64_t *primes = NULL;            /* list of primes up to `limit`     */
static idx_t *spf = NULL;                  /* smallest prime factor table     */

seq_t a(idx_t n)
{
    if (!initialized) {
        build_spf(limit, &spf);
        build_primes_upto(limit, &primes, &prime_cnt);
        initialized = true;
    }

    /* the sequence is defined for n ≥ 1; below that we return 0 */
    if (n == 0) return 0;

    seq_t count = 0;
    idx_t two_n = n << 1;                  /* 2 * n */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p <= n) continue;              /* p must be > n   */
        if (p > two_n - 2) break;          /* g = 2n - p ≥ 2 */

        idx_t g = two_n - p;
        if (g < 2) continue;
        if (!is_prime(g)) continue;        /* g must be prime */

        /* primitive‑root test: g^( (p−1)/q ) mod p ≠ 1 for all prime q | (p−1) */
        bool is_root = true;
        idx_t phi = p - 1;
        idx_t temp = phi;
        idx_t prev_q = 0;

        while (temp > 1) {
            idx_t q = spf[temp];
            if (q != prev_q) {
                if (mod_pow((seq_t)g, (uint64_t)(phi / q), (seq_t)p) == 1) {
                    is_root = false;
                    break;
                }
                prev_q = q;
            }
            temp /= q;
        }

        if (is_root) count++;
    }
    return count;
}
