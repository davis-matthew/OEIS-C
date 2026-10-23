/*
 * A105913  Primes for which -41 is a primitive root.
 * https://oeis.org/A105913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, pow_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the primes p for which -41 is a primitive root modulo p
 * (OEIS A105913).  The memoised function a(n) returns the n‑th such prime
 * (n ≥ 1).  It works for any supported seq_t type.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"
#include "../utils/pow_mod.h"

/* Test whether -41 is a primitive root modulo prime p */
static bool is_minus41_primitive_root(uint64_t p)
{
    /* (-41) mod p */
    uint64_t base = (p - (41 % p)) % p;    /* 0 < base < p for any odd p, base==1 for p==2 */
    if (base == 0) return false;           /* this cannot happen for a prime */
    uint64_t phi = p - 1;                  /* Euler's totient of a prime */
    uint64_t m = phi;
    uint64_t d = 2;
    while (d <= m / d) {
        if (m % d == 0) {
            /* If base^(phi/d) ≡ 1 (mod p), then d divides the order */
            if (pow_mod(base, phi / d, p) == 1) return false;
            while (m % d == 0) m /= d;
        }
        d = (d == 2) ? 3 : d + 2;   /* test only 2 and odd divisors */
    }
    if (m > 1) {                      /* remaining prime factor of phi */
        if (pow_mod(base, phi / m, p) == 1) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t count = 0;
    static size_t cap = 0;
    static uint64_t next = 2;          /* next candidate to test (odd numbers after 2) */

    if (n == 0) return 0;              /* undefined for offset 0 */

    while (count < n) {
        uint64_t p = next;

        /* Find the next prime starting from p */
        while (!is_prime(p)) {
            if (p == 2)
                p = 3;
            else
                p += 2;               /* skip even numbers */
        }

        if (is_minus41_primitive_root(p)) {
            /* store the term */
            if (count >= cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {          /* allocation failure – cannot recover */
                    return 0;
                }
                terms = tmp;
                cap = new_cap;
            }
            terms[count++] = (seq_t)p;
        }

        /* Prepare the next candidate after the found prime 'p' */
        next = (p == 2) ? 3 : p + 2;   /* keep only odd candidates after 2 */
    }

    return terms[n - 1];
}
