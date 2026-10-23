/*
 * A040054  Primes p such that x^3 = 10 has a solution mod p.
 * https://oeis.org/A040054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) returns the n-th prime p such that the congruence x^3 ≡ 10
 *            (mod p) has an integer solution. For a prime p≠2,5 this holds
 *            iff - p ≡ 2 (mod 3) (automatic), or - p ≡ 1 (mod 3) and
 *            10^((p-1)/3) ≡ 1 (mod p). The primes 2,3,5 also satisfy the
 *            condition
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t *terms = NULL;      /* sequence values */
static size_t terms_size = 0;    /* number of elements stored */
static size_t terms_cap  = 0;    /* allocated capacity */
static uint64_t last_checked = 0;   /* last integer examined for primality */

static void ensure_capacity(void)
{
    if (terms_size + 1 > terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failure */
        terms = tmp;
        terms_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but safe fallback */

    /* Generate sequence until we have at least n terms. */
    for (uint64_t cand = last_checked + 1; terms_size < n; ++cand) {
        if (!is_prime(cand)) continue;

        bool qualifies = false;
        if (cand == 2 || cand == 5)
            qualifies = true;
        else if (cand % 3 == 2)
            qualifies = true;
        else {
            uint64_t exp = (cand - 1) / 3;
            seq_t res = mod_pow((seq_t)10, exp, (seq_t)cand);
            if (res == (seq_t)1)
                qualifies = true;
        }

        if (qualifies) {
            ensure_capacity();
            terms[terms_size++] = (seq_t)cand;
        }

        last_checked = cand;
    }

    return terms[n - 1];
}
