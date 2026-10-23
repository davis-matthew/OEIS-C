/*
 * A038892  Primes p such that 19 is not a square mod p.
 * https://oeis.org/A038892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_small, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the first n primes p such that 19 is not a quadratic residue modulo p. */
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_small.h"
#include "../utils/mod_pow.h"
#include "../utils/grow_seq.h"

static uint64_t s_next_candidate = 2;   /* next number to test for primality   */
static seq_t *s_terms = NULL;           /* array of accepted primes            */
static size_t s_terms_cnt = 0;           /* number of primes stored             */
static size_t s_terms_cap = 0;           /* capacity of s_terms                 */

/* Ensure the array s_terms contains at least n elements. */
static void ensure_n_terms(idx_t n)
{
    while (s_terms_cnt < n)
    {
        uint64_t candidate = s_next_candidate;

        /* Find the next prime satisfying the Legendre condition.      */
        while (true)
        {
            if (candidate == 2)                    /* Special case      */
            {
                break;
            }
            else if ((candidate & 1U) && is_prime_small(candidate))
            {
                if (candidate != 19)               /* exclude p = 19    */
                {
                    seq_t res = mod_pow((seq_t)19,
                                        (candidate - 1) / 2,
                                        candidate);
                    if (res == (candidate - 1))    /* 19 is non‑residue */
                    {
                        break;
                    }
                }
            }
            candidate++;                           /* next candidate    */
        }

        /* Add the found prime to the array.                      */
        if (s_terms_cnt >= s_terms_cap)
            grow_seq(&s_terms, &s_terms_cap, s_terms_cnt + 1);

        s_terms[s_terms_cnt++] = (seq_t)candidate;
        s_next_candidate = candidate + 1;           /* prepare for next search */
    }
}

/* Return the n-th element of A038892 (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                            /* undefined, safety    */
    ensure_n_terms(n);
    return s_terms[n - 1];
}
