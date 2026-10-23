/*
 * A040978  Primes p such that x^5 = 17 has a solution mod p.
 * https://oeis.org/A040978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) lists primes p such that x^5 ≡ 17 (mod p) has a solution.
 *            We test each prime p by verifying whether 17^((p-1)/gcd(5,p-1))
 *            ≡ 1 (mod p). For p = 17 (the only prime divisor of 17) the
 *            congruence is true because 0 is a 5th power mod 17
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t size = 0;
    static size_t cap = 0;
    static idx_t candidate = 2;

    while (size < n) {
        uint64_t p = (uint64_t)candidate;
        /* skip even numbers greater than 2 */
        if (p > 2 && (p & 1) == 0) {
            candidate++;
            continue;
        }
        if (is_prime((idx_t)p)) {
            /* Special case: p divides 17 */
            if (p == 17U) {
                /* Ensure capacity */
                if (size == cap) {
                    size_t new_cap = (cap == 0) ? 16 : cap * 2;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) return (seq_t)0;
                    cache = new_cache;
                    cap = new_cap;
                }
                cache[size++] = (seq_t)p;
            } else {
                seq_t g = gcd((seq_t)5, (seq_t)(p - 1));
                uint64_t exp = (p - 1) / (uint64_t)g;
                seq_t r = mod_pow((seq_t)17, exp, (seq_t)p);
                if (r == (seq_t)1) {
                    if (size == cap) {
                        size_t new_cap = (cap == 0) ? 16 : cap * 2;
                        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!new_cache) return (seq_t)0;
                        cache = new_cache;
                        cap = new_cap;
                    }
                    cache[size++] = (seq_t)p;
                }
            }
        }
        candidate++;
    }
    return cache[n - 1];
}
