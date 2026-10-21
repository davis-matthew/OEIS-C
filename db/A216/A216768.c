/*
 * A216768  Primes p such that x^46 = -2 has no solution mod p.
 * https://oeis.org/A216768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A216768: primes p such that x^46 = -2 has no solution mod p.
 * The test is: if (-2)^( (p-1)/gcd(46,p-1) ) ≠ 1 (mod p) then p is in the
 * sequence.  This function finds the n‑th such prime, counting from a(1)=5.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/u64_gcd.h"

seq_t a(idx_t n)
{
    /* Static state (memoised) ------------------------------------------ */
    static uint64_t *primes   = NULL; /* all primes found so far          */
    static size_t   primes_cap = 0, primes_cnt = 0;
    static uint64_t *special  = NULL; /* primes satisfying the property */
    static size_t   special_cap = 0, special_cnt = 0;
    static uint64_t candidate  = 3;   /* next odd candidate to test     */

    /* Initialise structures on first call ------------------------------ */
    if (!primes) {
        primes_cap = 256;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
        if (!primes) abort();
        primes[0] = 2;
        primes_cnt = 1;
    }
    if (!special) {
        special_cap = 64;
        special = (uint64_t *)malloc(special_cap * sizeof(uint64_t));
        if (!special) abort();
        special_cnt = 0;
    }

    /* Return immediately if we already have enough specials. --------- */
    if (special_cnt >= (size_t)n)
        return (seq_t)special[n-1];

    /* Main loop: generate primes and test the residue condition      */
    while (special_cnt < (size_t)n) {
        /* Quick primality test by trial division using known primes. */
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime in the list. */
            if (primes_cnt >= primes_cap) {
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_cnt++] = candidate;

            /* Test the 46th‑power residuosity condition. */
            uint64_t g = u64_gcd(46ULL, candidate-1);
            uint64_t e = (candidate-1) / g;
            seq_t base = (seq_t)(candidate-2u);                     /* -2 mod p   */
            seq_t res  = mod_pow(base, e, (seq_t)candidate);
            if (res != (seq_t)1) {                                  /* no solution */
                if (special_cnt >= special_cap) {
                    special_cap *= 2;
                    uint64_t *tmp = (uint64_t *)realloc(special,
                                                        special_cap * sizeof(uint64_t));
                    if (!tmp) abort();
                    special = tmp;
                }
                special[special_cnt++] = candidate;
            }
        }

        candidate += 2;  /* proceed to the next odd number            */
    }

    return (seq_t)special[n-1];
}
