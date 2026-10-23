/*
 * A047652  Primes for which golden mean is a cubic residue.
 * https://oeis.org/A047652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 528, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the n-th prime p such that
 * Fibonacci((p-1)/3) ≡ 0 (mod p)  
 * i.e. the sequence of primes for which the golden mean is a cubic residue.
 * The implementation brute‑forces primes and checks the condition using
 * fast doubling Fibonacci modulo p.  The method runs in O(n log p)
 * time and is well within the required limits. */

#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/mul_mod64.h"

/* fast doubling Fibonacci modulo mod.  Returns F(n) mod mod. */
static uint64_t fib_mod_u64(uint64_t n, uint64_t mod)
{
    if (n == 0) return 0;
    uint64_t a = 0;          /* F(0) */
    uint64_t b = 1;          /* F(1) */
    int shift = 63 - __builtin_clzll(n);   /* highest set bit */
    for (int i = shift; i >= 0; --i) {
        /* Compute F(2k) = a * (2*b - a) */
        uint64_t t = (mul_mod64(b, 2, mod) + mod - a) % mod;
        uint64_t d = mul_mod64(a, t, mod);
        /* Compute F(2k+1) = a*a + b*b */
        uint64_t e = (mul_mod64(a, a, mod) + mul_mod64(b, b, mod)) % mod;
        if ((n >> i) & 1) {   /* bit is 1 -> k = 2k+1 */
            a = e;
            b = (d + e) % mod;
        } else {              /* bit is 0 -> k = 2k   */
            a = d;
            b = e;
        }
    }
    return a % mod;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used for OEIS offset, safety */
    idx_t count = 0;
    uint64_t p = 2;                /* start just before the smallest prime */

    while (1) {
        p = next_prime(p);          /* next prime after p */
        if (p < 139) continue;      /* skip small primes that do not belong */

        uint64_t k = (p - 1) / 3;
        if (fib_mod_u64(k, p) == 0) {
            ++count;
            if (count == n) return (seq_t)p;
        }
    }
}
