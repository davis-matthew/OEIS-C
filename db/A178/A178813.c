/*
 * A178813  a(n) = (prime(n)^(p-1) - 1)/p^2 mod p, where p is the first prime
 *          that divides (prime(n)^(p-1) - 1)/p.
 * https://oeis.org/A178813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = (prime(n)^(p-1) - 1)/p^2 mod p, where p is the first
 *            prime such that p^2 | prime(n)^(p-1) - 1
 * Range:     u8 none, u16 n <= 14, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

/* fast modular exponentiation for 64‑bit integers, using 128‑bit multiplication */
static uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1U)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1U;
    }
    return result;
}

/* Find the first prime p such that p^2 divides base^(p-1) - 1   */
static uint64_t first_wieferich(uint64_t base)
{
    for (uint64_t p = 2; ; ++p)
    {
        if (!is_prime(p))
            continue;
        if (pow_mod_u64(base, p - 1, p * p) == 1U)
            return p;
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1U)
        return (seq_t)0;                /* out‑of‑range requests */

    uint64_t base = (uint64_t)get_prime(n);   /* prime(n) */
    uint64_t p    = first_wieferich(base);    /* Wieferich prime for this base */

    uint64_t mod_p3 = p * p * p;              /* p³ fits in 64‑bit for the limits needed */
    uint64_t pow_mod = pow_mod_u64(base, p - 1, mod_p3);

    /* (base^(p-1)-1)/p² modulo p.  Since base^(p-1) ≡ 1 (mod p²),
       pow_mod = 1 + k·p² with k < p.  Thus ((pow_mod-1)/p²) % p = k.  */
    uint64_t quotient = (pow_mod - 1U) / (p * p);   /* quotient < p */

    return (seq_t)quotient;              /* quotient already < p */
}
