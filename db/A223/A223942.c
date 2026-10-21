/*
 * A223942  Least prime q such that (x^{p_n}-1)/(x-1) is irreducible modulo q,
 *          where p_n is the n-th prime.
 * https://oeis.org/A223942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime q such that the cyclotomic polynomial *
 *            (x^{p_n}-1)/(x-1) is irreducible modulo q. * This is equivalent
 *            to finding the smallest prime primitive root * of the n-th prime
 *            p_n (for p_n > 2; for p_n = 2 the answer is 2)
 * Range:     u8 n <= 54, u16 n <= 450, u32 n <= 450, u64 n <= 450,
 *            u128 n <= 450, big any n
 * Helpers:   factor_u64, get_prime, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/mod_pow.h"      /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */

/* Return the smallest prime primitive root of a prime p (>2). */
static seq_t least_prime_primitive_root(uint64_t p)
{
    if (p == 2)                  /* p_n = 2 has a(1)=a(2)=a(3)=2 */
        return (seq_t)2;

    /* Factor p-1 into distinct primes. */
    uint64_t fac[64], exc[64];
    size_t num_factors = 0;
    factor_u64(p - 1, fac, exc, &num_factors);   /* fac[i] holds a prime factor of p-1 */

    /* Search primes g, 2 <= g < p, that satisfy the primitive root test. */
    for (uint64_t g = 2; g < p; ++g) {
        if (!is_prime((idx_t)g))          /* only consider prime candidates */
            continue;

        bool ok = true;
        for (size_t i = 0; i < num_factors; ++i) {
            uint64_t q = fac[i];
            if (mod_pow((seq_t)g, (p - 1) / q, (seq_t)p) == (seq_t)1) {
                ok = false;
                break;
            }
        }
        if (ok)
            return (seq_t)g;              /* g is the smallest prime primitive root */
    }

    /* In practice this point is never reached: p itself is always a primitive root of itself. */
    return (seq_t)p;
}

seq_t a(idx_t n)
{
    if (n < 1)      /* defined only for n >= 1, return 0 otherwise */
        return (seq_t)0;

    uint64_t p = (uint64_t)get_prime(n);   /* nth prime p_n */
    return least_prime_primitive_root(p);
}
