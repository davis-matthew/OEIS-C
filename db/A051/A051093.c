/*
 * A051093  Primes p such that x^48 = -2 has a solution mod p.
 * https://oeis.org/A051093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime p (with offset 1) such that the congruence
   x^48 ≡ -2 (mod p) has a solution.  Equivalently, -2 is a 48‑th power residue
   modulo p.  For odd primes the condition is
        (-2)^{(p-1)/gcd(p-1,48)} ≡ 1 (mod p).
   For p = 2 the congruence holds trivially. */

#include "../utils/gcd.h"
#include "../utils/mod_pow.h"
#include "../utils/is_prime.h"
#include <stdlib.h>

/* dynamic array of found terms */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static size_t terms_cap = 0;
static idx_t next_candidate = 2;        /* next number to test for primality */

/* allocate more space for the terms array */
static void grow_terms(void)
{
    size_t new_cap = terms_cap ? terms_cap * 2 : 64;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                         /* allocation failure */
    terms = tmp;
    terms_cap = new_cap;
}

/* ensure that the first n terms have been generated */
static void ensure_terms(idx_t n)
{
    if (terms_cnt >= (size_t)n)
        return;

    while (terms_cnt < (size_t)n)
    {
        /* find next prime candidate */
        idx_t cand = next_candidate++;
        if (!is_prime(cand))
            continue;

        seq_t p = (seq_t)cand;

        /* special handling for p = 2 */
        if (cand == 2u)
        {
            if (terms_cnt == terms_cap)
                grow_terms();
            terms[terms_cnt++] = p;
            continue;
        }

        /* compute d = gcd(p-1,48) */
        uint64_t d = (uint64_t)gcd((seq_t)48, (seq_t)(cand - 1));

        /* exponent = (p-1)/d */
        uint64_t exp = (cand - 1) / d;

        /* base = -2 mod p = p - 2 */
        seq_t base = p - (seq_t)2;

        /* check if base^exp ≡ 1 (mod p) */
        seq_t res = mod_pow(base, exp, p);
        if (res == (seq_t)1)
        {
            if (terms_cnt == terms_cap)
                grow_terms();
            terms[terms_cnt++] = p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* offset is 1, return 0 for safety */

    ensure_terms(n);
    return terms[(size_t)n - 1];
}
