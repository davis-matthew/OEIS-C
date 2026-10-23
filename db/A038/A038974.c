/*
 * A038974  Primes p such that 86 is not a square mod p.
 * https://oeis.org/A038974
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038974: primes p such that 86 is not a quadratic residue modulo p
   (Legendre symbol (86|p) = -1) */

/* This implementation performs a straight‑forward search over odd
   integers, testing each for primality by trial division and
   evaluating the Legendre symbol via repeated squaring.  It
   works for any seq_t / idx_t combination that can represent
   64‑bit integers.  The search is efficient enough for the first
   ten thousand terms and requires no external utilities. */

#include <stdint.h>
#include <stdbool.h>

/* Simple static primality test for 64‑bit numbers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Modular exponentiation: base^exp mod mod */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1;
    base %= mod;
    while (exp)
    {
        if (exp & 1) res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based. For n == 0 the behaviour is undefined. */
    idx_t found = 0;
    uint64_t candidate = 3; /* start with the first odd prime */

    while (1)
    {
        if (is_prime_uint64(candidate))
        {
            /* If 86 is a quadratic residue modulo candidate, the
               Legendre symbol equals 1.  The Legendre symbol is
               −1 (non‑residue) iff candidate divides 86^((p-1)/2) − 1. */
            uint64_t leg = mod_pow_uint64(86, (candidate - 1) / 2, candidate);
            if (leg == candidate - 1)   /* non‑residue */
            {
                ++found;
                if (found == n) return (seq_t)candidate;
            }
        }
        candidate += 2; /* advance to next odd number */
    }
}
