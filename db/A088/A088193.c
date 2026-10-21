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
 * Helpers:   jacobi_symbol, next_prime
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
#include "../utils/jacobi_symbol.h"
#include "../utils/next_prime.h"

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
