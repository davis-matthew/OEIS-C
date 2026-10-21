/*
 * A081617  Smallest k such that (product of first n primes)*k+1 is divisible
 *          by the (n+1)-th prime. Also (A075306(n)-1)/A002110(n).
 * https://oeis.org/A081617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest k such that (product of the first n primes)*k + 1 is divisible by the (n+1)-th prime. */
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* The (n+1)-th prime */
    uint32_t next_prime = get_prime(n + 1);
    seq_t p = (seq_t)next_prime;

    /* Product of the first n primes modulo p */
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        prod = (prod * (seq_t)get_prime(i)) % p;
    }

    /* Modular inverse of prod modulo p (p is prime) */
    uint64_t exp = (uint64_t)p - 2ULL;
    seq_t inv = mod_pow(prod, exp, p);

    /* Smallest positive k */
    seq_t k = ((p - 1) * inv) % p;
    return k;
}
