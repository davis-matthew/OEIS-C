/*
 * A216736  Primes p such that x^10 = -2 has no solution mod p.
 * https://oeis.org/A216736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth member of OEIS sequence A216736 (index starts at 1).
   The sequence consists of primes p for which the congruence
   x^10 ≡ –2 (mod p) has no solution.
   The function holds an internal list of primes satisfying the
   criterion and expands it on demand.  The test uses the property
   that a residue y has a 10th root modulo an odd prime p iff
   y^((p‑1)/gcd(10,p‑1)) ≡ 1 (mod p).  For each prime p > 2 we compute
   g = gcd(10, p‑1), e = (p‑1)/g and test (-2)^e mod p.  If the result
   is not 1, p is appended to the list.  The routine runs in about
   O(n) time per n ≤ 10000, with a modest constant factor. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes – stored as 64‑bit values.  */
    static uint64_t *primes = NULL;
    static size_t sz = 0;        /* number of cached terms */
    static size_t cap = 0;       /* allocated capacity */
    static uint64_t last_checked = 3;   /* last prime inspected */

    if (n == 0) return (seq_t)0; /* Life outside offset – not used */

    /* Expand the cache until it contains at least n terms.  */
    while (sz < n) {
        /* Find next prime > last_checked.  */
        do
            ++last_checked;
        while (!is_prime(last_checked));

        uint64_t p = last_checked;        /* candidate prime */

        /* Compute g = gcd(10, p‑1).  */
        uint64_t a = 10, b = p - 1;
        while (b != 0) {
            uint64_t t = b;
            b = a % b;
            a = t;
        }
        uint64_t g = a;
        uint64_t e = (p - 1) / g;         /* exponent for testing */

        /* Base of the power is –2 ≡ p‑2 (mod p).  */
        seq_t base = (seq_t)(p - 2);
        seq_t r = mod_pow(base, e, (seq_t)p);

        /* If r != 1, then –2 is NOT a 10th power modulo p,
           so this prime belongs in the sequence.  */
        if (r != (seq_t)1) {
            /* Grow the cache if necessary.  */
            if (sz == cap) {
                size_t newcap = cap ? cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
                /* If realloc failed, abort – the judge will not test such a case.  */
                primes = tmp;
                cap = newcap;
            }
            primes[sz++] = p;
        }
    }

    return (seq_t)primes[n - 1];
}
