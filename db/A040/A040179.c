/*
 * A040179  Primes p such that x^5 = 11 has a solution mod p.
 * https://oeis.org/A040179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A040179: primes p for which the congruence x^5 = 11 (mod p) is solvable.
   Criterion: if p ≠ 5 and p−1 is not a multiple of 5, then the homomorphism
   x ↦ x^5 is bijective on the multiplicative group and every a has a 5th
   root.  If 5 | (p−1), then a has a 5th root iff a^((p−1)/5) ≡ 1 (mod p),
   except that a = 0 (i.e. p divides 11) always has the trivial root x = 0.
   The function generates primes by trial division and caches the sequence
   for fast repeated calls. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* fast modular exponentiation for uint64_t arguments to avoid overflow */
static uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* index starts at 1 */

    /* static cache of found sequence values */
    static seq_t *result = NULL;
    static idx_t rsize = 0, rcap = 0;

    /* static list of primes used for trial division      */
    static uint64_t *primes = NULL;
    static idx_t pcnt = 0, pcap = 0;

    /* current candidate prime (always odd after 2) */
    static uint64_t candidate = 2;

    /* generate terms until we have at least n elements */
    while (rsize < n) {
        /* test candidate for primality using stored primes */
        bool is_prime = true;
        for (idx_t i = 0; i < pcnt &&
                (uint64_t)primes[i] * (uint64_t)primes[i] <= candidate; ++i) {
            if (candidate % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* store prime in primes[] */
            if (pcnt == pcap) {
                pcap = pcap ? pcap << 1 : 16;
                uint64_t *tmp = (uint64_t*)realloc(primes, pcap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[pcnt++] = candidate;

            /* check the 5th‑power residue condition */
            bool good;
            if (candidate == 11) {
                /* 11 ≡ 0 (mod 11) so x = 0 is a solution */
                good = true;
            } else if (candidate % 5 != 1) {
                good = true;
            } else {
                uint64_t exp = (candidate - 1) / 5;
                good = (mod_pow64(11ULL, exp, candidate) == 1);
            }

            if (good) {
                /* store in result cache */
                if (rsize == rcap) {
                    rcap = rcap ? rcap << 1 : 16;
                    seq_t *tmp = (seq_t*)realloc(result, rcap * sizeof(seq_t));
                    if (!tmp) abort();
                    result = tmp;
                }
                result[rsize++] = (seq_t)candidate;
            }
        }

        /* advance to next candidate */
        candidate = (candidate == 2) ? 3 : candidate + 2;   /* odd numbers only */
    }

    return result[n - 1];
}
