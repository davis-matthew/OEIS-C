/*
 * A040062  Primes p such that x^3 = 12 has a solution mod p.
 * https://oeis.org/A040062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes primes p such that x^3 = 12 has a solution modulo p.
   For p = 2,3 the solution is trivial (12 ≡ 0 mod p).
   For primes p ≡ 2 mod 3 every element has a cube root in
   ℤ/pℤ, so p is always included.
   For primes p ≡ 1 mod 3 we test whether 12^((p-1)/3) ≡ 1 (mod p). */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* cache of sequence values */
    static size_t seq_len = 0, seq_cap = 0;
    static uint64_t candidate = 2; /* next number to test for primality */

    /* generate terms until we have at least n of them */
    while (seq_len < n) {
        /* find next prime */
        while (!is_prime(candidate))
            ++candidate;

        /* decide if this prime belongs to the sequence */
        bool in_seq = (candidate == 2) ||
                      (candidate == 3) ||
                      (candidate % 3 == 2);

        if (!in_seq) {  /* candidate % 3 == 1 */
            unsigned int exp = (unsigned int)((candidate - 1) / 3);
            seq_t mod = (seq_t)candidate;
            seq_t r = mod_pow((seq_t)12, exp, mod);
            in_seq = (r == (seq_t)1);
        }

        if (in_seq) {
            if (seq_len == seq_cap) {              /* grow cache if necessary */
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)candidate;
        }
        ++candidate;                              /* next number to check */
    }

    return seq[n - 1];
}
