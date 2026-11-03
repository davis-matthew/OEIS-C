/*
 * A038887  15 is a square mod p.
 * https://oeis.org/A038887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th prime p such that 15 is a quadratic residue
 *            modulo p * (i.e., Legendre symbol (15|p) = 1 or 0, so 15 ≡
 *            0,1,4,… mod p). * Includes p=2 and primes dividing 15
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 981 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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
    if (n == 0) return 0;
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t last_checked = 1;

    while (primes_len < n) {
        for (idx_t cand = last_checked + 1; ; cand++) {
            if (is_prime(cand)) {
                int leg = jacobi_symbol(15, cand);
                if (leg >= 0) {               /* 15 is a quadratic residue mod cand */
                    if (primes_len + 1 > primes_cap) {
                        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                        seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                        if (!tmp) abort();
                        primes = tmp;
                        primes_cap = new_cap;
                    }
                    primes[primes_len++] = (seq_t)cand;
                }
            }
            if (primes_len >= n) {
                last_checked = cand;
                break;
            }
        }
    }
    return primes[n - 1];
}
