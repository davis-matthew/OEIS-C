/*
 * A059336  Primes p such that x^34 = 2 has no solution mod p.
 * https://oeis.org/A059336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A059336:
   Primes p such that the congruence x^34 ≡ 2 (mod p) has no solution.
   For an odd prime p, let d = gcd(p‑1,34) (which is 2 or 34).
   The equation has a solution iff 2^{(p‑1)/d} ≡ 1 (mod p).
   Hence p belongs to the sequence iff 2^{(p‑1)/d} mod p ≠ 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static storage for primes and sequence terms ---------------------- */
    static uint64_t *prime_list = NULL;   /* prime_list[0] = 2              */
    static size_t  prime_cnt   = 0;
    static size_t  prime_cap   = 0;
    static uint64_t candidate  = 3;       /* next odd number to test       */

    static seq_t *term_list   = NULL;     /* sequence terms                */
    static size_t  term_cnt   = 0;
    static size_t  term_cap   = 0;

    if (n == 0) return (seq_t)0;          /* defined as 0 for safety       */

    /* initialise prime list with 2 --------------------------------------- */
    if (prime_cnt == 0) {
        prime_cap   = 8;
        prime_list  = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt   = 1;
        prime_list[0] = 2;
    }

    /* generate primes and terms until we have n terms --------------------- */
    while (term_cnt < (size_t)n) {
        uint64_t p = candidate;

        /* test primality of p using known primes ------------------------ */
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t q = prime_list[i];
            if (q * q > p) break;
            if (p % q == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            /* append p to prime list ----------------------------------- */
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 8;
                prime_list = (uint64_t *)realloc(prime_list,
                                                 prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = p;

            /* compute gcd(p-1,34) --------------------------------------- */
            uint64_t d = ((p - 1) % 17 == 0) ? 34 : 2;
            uint64_t e = (p - 1) / d;

            /* evaluate 2^e (mod p) ------------------------------------- */
            seq_t res = mod_pow((seq_t)2, e, (seq_t)p);

            /* if not 1, p is a term of the sequence --------------------- */
            if (res != (seq_t)1) {
                if (term_cnt == term_cap) {
                    term_cap = term_cap ? term_cap * 2 : 8;
                    term_list = (seq_t *)realloc(term_list,
                                                 term_cap * sizeof(seq_t));
                }
                term_list[term_cnt++] = (seq_t)p;
            }
        }

        candidate += 2;   /* only odd numbers need be tested */
    }

    return term_list[n - 1];
}
