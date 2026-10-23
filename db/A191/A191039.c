/*
 * A191039  Primes that are squares mod 61.
 * https://oeis.org/A191039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p (p ≠ 61) such that p³⁰ ≡ 1 (mod 61)
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"


seq_t a(idx_t n)
{
    static seq_t *res = NULL;          /* array of qualifying primes       */
    static size_t res_len = 0, res_cap = 0;

    static uint64_t *prime_list = NULL;/* primes found so far for trial div*/
    static size_t prime_len = 0, prime_cap = 0;

    static uint64_t candidate = 3;      /* next odd number to test         */
    static bool initialized = false;

    if (!initialized) {
        res_cap = 64;
        res = (seq_t*)malloc(res_cap * sizeof(seq_t));
        prime_cap = 64;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        initialized = true;
    }

    size_t target = (size_t)n;          /* zero‑based index of desired term */

    while (res_len < target) {
        /* primality test using stored primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if ((unsigned __int128)p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store candidate for future divisions */
            if (prime_len == prime_cap) {
                prime_cap = prime_cap * 2 + 16;
                prime_list = (uint64_t*)realloc(prime_list, prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_len++] = candidate;

            /* check quadratic residue modulo 61 */
            if (candidate % 61 != 0) {
                seq_t base = (seq_t)(candidate % 61);
                seq_t mod61 = (seq_t)61;
                seq_t leg = mod_pow(base, (uint64_t)30, mod61);
                if (leg == (seq_t)1) {
                    if (res_len == res_cap) {
                        res_cap = res_cap * 2 + 16;
                        res = (seq_t*)realloc(res, res_cap * sizeof(seq_t));
                    }
                    res[res_len++] = (seq_t)candidate;
                }
            }
        }

        candidate += 2;   /* only odd candidates (starting from 3) */
    }

    return res[target - 1];
}
