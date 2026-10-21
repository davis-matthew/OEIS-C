/*
 * A220093  a(n) is the smallest odd positive integer > 1 that makes the
 *          composite number 2*n-a(n)*p1 divisible by p2, where p1 and p2 are
 *          the smallest and 2nd smallest odd prime numbers that are not
 *          factors of n. When no such odd positive integer exists, a(n)=0.
 * https://oeis.org/A220093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2116, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A220093: for each n, find the smallest odd integer a>1 such that
 *   c1 = a * p1   (p1 is the smallest odd prime not dividing n)
 *   c2 = 2*n - c1
 * satisfy:
 *   p2 divides c2   (p2 is the second smallest odd prime not dividing n)
 *   c2 is composite
 *   gcd(c1, c2) = 1
 *   a is odd and >1
 * If no such a exists, return 0.
 * The implementation searches the arithmetic progression of admissible a
 * values and checks the conditions sequentially.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"        /* is_prime(idx_t) */
#include "../utils/isPrimeGeneric.h"  /* isPrimeGeneric(seq_t) */

/* Extended Euclidean algorithm: return inverse of a modulo mod, 0 if none. */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t r0 = (int64_t)mod, r1 = (int64_t)a;
    int64_t t0 = 0, t1 = 1;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t tmp = r0 - q * r1; r0 = r1; r1 = tmp;
        tmp = t0 - q * t1; t0 = t1; t1 = tmp;
    }
    if (r0 != 1) return 0;               /* inverse does not exist */
    if (t0 < 0) t0 += (int64_t)mod;
    return (uint64_t)t0;
}

/* Euclidean gcd for 64‑bit integers (used for coprimality test). */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Find the first two odd primes not dividing n. */
    uint64_t p1 = 0, p2 = 0, candidate = 3;
    while (p2 == 0) {
        if (is_prime(candidate)) {
            if (n % candidate != 0) {
                if (p1 == 0)
                    p1 = candidate;
                else
                    p2 = candidate;
            }
        }
        candidate += 2;     /* skip even numbers */
    }

    /* Compute modular inverse of p1 modulo p2. */
    uint64_t inv_p1 = modinv_uint64(p1, p2);
    if (inv_p1 == 0) return 0;              /* should not happen */

    /* Smallest non‑negative solution a0 modulo p2. */
    uint64_t n_mod_p2 = (uint64_t)(n % p2);
    uint64_t a0 = ((n_mod_p2 * 2ULL) % p2 * inv_p1) % p2;

    /* Upper bound for a ensuring 2*n - a*p1 > 1. */
    idx_t max_a = ((idx_t)2 * n - 2) / p1;

    /* Search the arithmetic progression a = a0 + k*p2. */
    for (uint64_t a_val = a0; a_val <= (uint64_t)max_a; a_val += p2) {
        if (a_val <= 1) continue;
        if ((a_val & 1ULL) == 0) continue;   /* only odd a */

        /* Compute c1 and c2. */
        seq_t c1 = (seq_t)a_val * (seq_t)p1;
        seq_t c2 = (seq_t)((idx_t)2 * n) - c1;

        if (c2 <= 1) continue;               /* must be >1 */
        if (isPrimeGeneric(c2)) continue;      /* must be composite */

        /* Check coprimality: gcd(c1, c2) must be 1. */
        if (gcd64((uint64_t)c1, (uint64_t)c2) != 1ULL) continue;

        return (seq_t)a_val;   /* found the minimal valid a */
    }

    return 0;   /* no suitable odd a found */
}
