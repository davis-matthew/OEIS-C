/*
 * A038963  Primes p such that 78 is a square mod p.
 * https://oeis.org/A038963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = nth prime p such that 78 is a quadratic residue modulo p. */
/* The function generates primes on demand and keeps a static cache of
   all primes found so far.  For each odd prime p it checks whether
   78^((p-1)/2) ≡ 1 (mod p); if so p is appended to the cache.  Primes
   2, 3 and 13 (which divide 78) are automatically accepted. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/append_candidate.h"
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t *cache = NULL;
    static size_t len   = 0;   /* number of primes stored */
    static size_t cap   = 0;   /* capacity of cache    */
    static idx_t  next  = 2;   /* next integer to test */

    /* generate terms until we have at least n entries */
    while (len < n) {
        /* handle prime 2 (always a square, as 78 ≡ 0 mod 2) */
        if (next == 2) {
            append_candidate(&cache, &len, &cap, (seq_t)2);
            next = 3;
            continue;
        }

        if (is_prime(next)) {
            /* primes 3 and 13 divide 78, so 78 is trivially a square mod them */
            if ((seq_t)78 % next == 0) {
                append_candidate(&cache, &len, &cap, next);
            } else {
                /* test Legendre symbol via Euler's criterion:
                   78^((p-1)/2) ≡ (78/p) (mod p)                */
                uint64_t exp = ((uint64_t)next - 1) / 2;
                seq_t residue = mod_pow((seq_t)78, exp, next);
                if (residue == (seq_t)1) {
                    append_candidate(&cache, &len, &cap, next);
                }
            }
        }
        ++next;
    }

    return cache[n - 1];
}
