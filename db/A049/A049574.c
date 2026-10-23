/*
 * A049574  Primes p such that x^42 = 2 has a solution mod p.
 * https://oeis.org/A049574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_int, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Criterion: 2 is a 42nd power residue modulo p.
 * For a prime p > 2, the congruence x^42 ≡ 2 (mod p) has a solution iff
 *      2^((p‑1)/gcd(42, p‑1)) ≡ 1 (mod p).
 *  p = 2 satisfies the congruence by definition (0^42 ≡ 2 (mod 2)).
 * This function returns the n‑th prime that satisfies the above condition
 * (offset 1 as in OEIS).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/gcd_int.h"

/* Small integer gcd. */

/* Binary modular exponentiation that works for any seq_t size. */
static inline seq_t mod_pow_big(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp != 0) {
        if ((exp & 1) != 0)
            result = (result * b) % mod;
        exp = exp >> 1;
        if (exp != 0)
            b = (b * b) % mod;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence starts at index 1 */

    static seq_t *primes = NULL;   /* cache of found primes */
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t last_checked = 1;   /* prime predecessor of the first prime */

    if (primes == NULL) {
        cap = 16;
        primes = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    seq_t candidate = last_checked + 1;
    if (candidate == 1)          /* ensure we start at 2 */
        candidate = 2;

    while (len < n) {
        /* Make candidate odd after the prime 2. */
        if (candidate > 2 && (candidate & 1) == 0)
            candidate += 1;

        /* Find the next prime. */
        while (!is_prime_big(candidate))
            candidate += 2;

        bool ok;
        if (candidate == 2) {
            ok = true;          /* 2 always satisfies the condition */
        } else {
            int rem = (int)((candidate - 1) % 42);
            int d = gcd_int(42, rem);
            seq_t exp = (candidate - 1) / (seq_t)d;
            seq_t val = mod_pow_big((seq_t)2, exp, candidate);
            ok = (val == (seq_t)1);
        }

        if (ok) {
            if (len == cap) {
                cap <<= 1;
                primes = (seq_t *)realloc(primes, cap * sizeof(seq_t));
            }
            primes[len++] = candidate;
        }

        last_checked = candidate;
        candidate = candidate + (candidate == 2 ? 1 : 2);
    }

    return primes[n - 1];
}
