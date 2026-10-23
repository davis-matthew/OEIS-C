/*
 * A167135  Primes congruent to {2, 3, 5, 7, 11} mod 12.
 * https://oeis.org/A167135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A167135.
   Terminology: a(n) = the n‑th prime p such that p mod 12 ∈ {2,3,5,7,11}.
   The function generates primes incrementally, testing each candidate
   by trial division with previously found primes and records only those
   that satisfy the required residue.  It caches results so that calls
   in any order are handled efficiently. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static cache of primes with required residues */
    static seq_t *residues = NULL;
    static size_t res_cap   = 0, res_cnt = 0;

    /* auxiliary list of all primes (needed for trial division) */
    static seq_t *prime_list = NULL;
    static size_t prime_cap   = 0, prime_cnt = 0;

    /* keep track of the last candidate examined */
    static uint64_t last_checked = 1;

    /* Grow cache until we have at least n terms */
    while (res_cnt < n) {
        uint64_t cand;

        /* Determine next candidate to test */
        if (last_checked == 1)           cand = 2;
        else if (last_checked == 2)      cand = 3;
        else if (last_checked == 3)      cand = 5;
        else                              cand = last_checked + 2;  /* only odd numbers */

        last_checked = cand;

        /* Test primality by trial division using prime_list */
        int isprime = 1;
        if (cand < 2) {
            isprime = 0;
        } else {
            uint64_t limit = (uint64_t)(sqrt((double)cand) + 1);
            for (size_t i = 0; i < prime_cnt; ++i) {
                uint64_t p = (uint64_t)prime_list[i];
                if (p > limit) break;
                if (cand % p == 0) { isprime = 0; break; }
            }
        }

        if (isprime) {
            /* store prime in prime_list */
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_list,
                                              new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0; /* allocation failure */
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = (seq_t)cand;

            /* check residue */
            uint64_t r = cand % 12;
            if (r == 2 || r == 3 || r == 5 || r == 7 || r == 11) {
                if (res_cnt == res_cap) {
                    size_t new_cap = res_cap ? res_cap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(residues,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) return (seq_t)0; /* allocation failure */
                    residues = tmp;
                    res_cap = new_cap;
                }
                residues[res_cnt++] = (seq_t)cand;
            }
        }
    }

    return residues[n - 1];
}
