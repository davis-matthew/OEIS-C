/*
 * A038984  Primes p such that 94 is not a square mod p.
 * https://oeis.org/A038984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that 94 is a quadratic non‑residue modulo p */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Return true if 94 is a quadratic non‑residue modulo the odd prime p. */
static bool not_residue(idx_t p)
{
    /* Legendre symbol (94|p) = 94^{(p−1)/2} (mod p).
       We return true when the result equals p−1 (i.e., -1). */
    idx_t exp = (p - 1) / 2;
    seq_t r = mod_pow((seq_t)94, exp, (seq_t)p);
    return r == (seq_t)p - 1;
}

/* a(n): the n‑th prime p (n ≥ 1) for which 94 is not a quadratic residue mod p. */
seq_t a(idx_t n)
{
    /* Memoised list of primes discovered so far. */
    static idx_t *primes = NULL;
    static size_t plist = 0, pcap = 0;

    /* Memoised list of "good" primes (satisfying the non‑residue property). */
    static seq_t *good = NULL;
    static size_t gsize = 0, gcap = 0;

    /* Next odd candidate to test for primality. */
    static idx_t next_candidate = 3;

    /* Populate until we have at least n good primes. */
    while (gsize < n) {
        idx_t cand = next_candidate;
        bool prime = true;

        /* Check divisibility by known primes up to sqrt(cand). */
        for (size_t i = 0; prime && i < plist; ++i) {
            idx_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                prime = false;
            }
        }

        if (prime) {
            /* Store the prime. */
            if (plist == pcap) {
                pcap = pcap ? (pcap << 1) : 16;
                primes = (idx_t *)realloc(primes, pcap * sizeof(idx_t));
            }
            primes[plist++] = cand;

            /* Check the Legendre condition. */
            if (not_residue(cand)) {
                if (gsize == gcap) {
                    gcap = gcap ? (gcap << 1) : 16;
                    good = (seq_t *)realloc(good, gcap * sizeof(seq_t));
                }
                good[gsize++] = (seq_t)cand;
            }
        }

        /* Advance to the next odd number. */
        next_candidate += 2;
    }

    return good[n - 1];
}
