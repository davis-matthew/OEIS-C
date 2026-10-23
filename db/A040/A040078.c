/*
 * A040078  Primes p such that x^3 = 17 has a solution mod p.
 * https://oeis.org/A040078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Condition: prime p such that 17 is a cubic residue modulo p
   i.e. p=2 or p=3 or p≡2 (mod 3) or (p≡1 (mod 3) and 17^((p–1)/3)≡1 (mod p)) */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* static mutable cache */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0, terms_cnt = 0;
    static uint64_t last_candidate = 1;            /* start before 2 */

    /* extend cache until we have at least n terms */
    while (terms_cnt < (size_t)n)
    {
        uint64_t p = ++last_candidate;

        /* test primality */
        if (!is_prime_big(p))
            continue;

        /* decide if p satisfies the condition */
        bool ok = false;
        if (p == 2u || p == 3u)
            ok = true;
        else if (p % 3u == 2u)
            ok = true;
        else
        {
            seq_t res = mod_pow((seq_t)17, (p - 1u) / 3u, p);
            if (res == (seq_t)1)
                ok = true;
        }

        if (ok)
        {
            /* grow array if needed */
            if (terms_cap == 0)
            {
                terms_cap = 16;
                terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            }
            else if (terms_cnt == terms_cap)
            {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)p;
        }
    }

    return terms[n - 1];
}
