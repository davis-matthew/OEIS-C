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
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"

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
                        seq_t *tmp = (seq_t *)realloc(primes,
                                                      new_cap * sizeof(seq_t));
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
