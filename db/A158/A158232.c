/*
 * A158232  Numbers which yield primes when "13" is prefixed or appended: N
 *          natural number is a member of the sequence, if P="13N" (prefixed
 *          13) and A="N13" (appended 13) are prime.
 * https://oeis.org/A158232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 23, u32 n <= 30, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A158232: numbers N such that 13*N and N*100+13 are prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digits.h"

/* Return the n-th term (n ≥ 1) of OEIS A158232 */
seq_t a(idx_t n)
{
    /* Cache of already found terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;   /* how many terms stored       */
    static size_t terms_cap = 0;   /* allocated capacity           */
    static idx_t nextN = 1;        /* next natural number to test */

    while (terms_len < n) {
        /* number of decimal digits of nextN */
        int d = digits((idx_t)nextN);
        seq_t pow10d = pow10((idx_t)d);                 /* 10^d */

        /* 13 * 10^d + nextN  → prefixed number */
        seq_t pref = ((seq_t)13) * pow10d + (seq_t)nextN;

        /* nextN * 100 + 13   → suffixed number */
        seq_t suff = (seq_t)nextN * ((seq_t)100) + ((seq_t)13);

        if (is_prime_big(pref) && is_prime_big(suff)) {
            /* grow cache array if needed */
            if (terms_len + 1 > terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 32;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* allocation failure */
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)nextN;
        }
        nextN++;
    }

    return terms[n - 1];
}
