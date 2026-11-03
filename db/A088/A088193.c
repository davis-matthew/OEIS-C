/*
 * A088193  Prime numbers where the sequence of largest quadratic residues
 *          modulo the primes (A088190) is non-monotonic.
 * https://oeis.org/A088193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 422, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9994 (2026-11-01)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A088193 – primes for which the largest quadratic residue
 *  modulo the prime is non‑increasing (A088190(p) ≤ A088190(prev)).
 *
 *  a(1) = 3, a(2) = 7, a(3) = 31, a(4) = 71, …
 *
 *  This implementation generates primes sequentially, evaluates
 *  the largest quadratic residue for each, and selects those where
 *  the value does not exceed the previous one.  The search is
 *  independent of the order of calls and runs in O(n) time for
 *  the first n terms (n ≈ 10 000 easily fits into 2 s).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

/* Jacobi symbol (a|n) for odd positive n <= 2^64-1.
   Returns 1, 0, or -1. */
static int jacobi_symbol(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    if (a == 0) return 0;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t n_mod8 = n & 7;
            if (n_mod8 == 3 || n_mod8 == 5)
                res = -res;
        }
        uint64_t tmp = a;
        a = n;
        n = tmp;
        if ((a & 3) == 3 && (n & 3) == 3)
            res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

/* Compute the largest quadratic residue modulo prime p.
 *  For p ≡ 1 (mod 4):   return p - 1
 *  For p ≡ 3 (mod 8):   return p - 2
 *  For p ≡ 7 (mod 8):   return p - 3   (found by searching smallest k
 *  with (p-k|p) = 1)
 */
static inline seq_t largest_qr(uint64_t p)
{
    if (p % 4ULL == 1ULL) {
        return (seq_t)(p - 1ULL);
    }
    if (p % 8ULL == 3ULL) {
        return (seq_t)(p - 2ULL);
    }
    /* p % 8 == 7 */
    uint64_t k = 1;
    while (jacobi_symbol(p - k, p) != 1) {
        ++k;
    }
    return (seq_t)(p - k);
}

/* Return the n‑th term of A088193.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;          /* undefined for n < 1 */
    }

    idx_t count = 0;                /* number of qualifying primes found */
    uint64_t p = 2;                 /* start just before the first odd prime */
    seq_t prev_qr = (seq_t)1;       /* A088190(2) = 1 */

    while (count < n) {
        p = next_prime(p);          /* next prime > current p */
        seq_t qr = largest_qr(p);   /* largest quadratic residue modulo p */

        if (qr <= prev_qr) {        /* non‑monotonic */
            ++count;
            if (count == n) {
                return (seq_t)p;    /* found the n‑th term */
            }
        }
        prev_qr = qr;               /* update for the next iteration */
    }

    return 0; /* unreachable, keeps the compiler happy */
}
