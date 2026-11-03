/*
 * A038948  Primes p such that 66 is not a square mod p.
 * https://oeis.org/A038948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 917 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038948 – primes p such that 66 is NOT a quadratic residue modulo p;
   the sequence is defined as the list of primes p with gcd(p,66)=1 and Legendre
   symbol (66|p) = –1.  */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

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

seq_t a(idx_t n)
{
    /* Static cache for the primes already found */
    static seq_t *primes = NULL;
    static size_t count = 0;       /* how many primes stored */
    static size_t cap   = 0;       /* allocated capacity */
    static idx_t last_checked = 1; /* candidate to start searching from */

    /* Generate primes until we have at least n of them */
    while (count < n) {
        /* Start from the first odd number ≥ 3 */
        if (last_checked < 3)
            last_checked = 3;
        else
            last_checked += 2;      /* skip even numbers */

        /* Find the next prime with (66|p) ≠ 1 and p ∤ 66 */
        while (true) {
            if (is_prime(last_checked)) {
                int ls = jacobi_symbol(66ULL, last_checked);
                if (ls != 0 && ls != 1) { /* 66 is not a square mod p */
                    /* Ensure the cache can hold another entry */
                    if (count == cap) {
                        size_t newcap = cap ? cap * 2 : 16;
                        seq_t *tmp = mem_resize(seq_t, primes, newcap);
                        if (!tmp) return 0;          /* allocation failed */
                        primes = tmp;
                        cap = newcap;
                    }
                    primes[count++] = (seq_t)last_checked;
                    break;                         /* prime found */
                }
            }
            last_checked += 2;                     /* skip even numbers */
        }
    }

    return primes[n - 1];
}
