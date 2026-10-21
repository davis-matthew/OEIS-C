/*
 * A066347  Numbers k such that (the sum of digits of k!)+1 is a prime.
 * https://oeis.org/A066347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth integer k such that the sum of the decimal digits of k!
   plus one is a prime.  The function lazily generates terms using an incremental
   factorial representation and an O(1) prime test on the digit sum.  All data
   is stored in static arrays and expanded as needed. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/prime_small.h"

/* --------------- Main sequence function --------------------------------- */
seq_t a(idx_t n) {
    /* static state */
    static seq_t *terms = NULL;          /* list of found k's               */
    static size_t terms_len = 0, terms_cap = 0;
    static unsigned char *fact = NULL;    /* factorial digits (little‑endian) */
    static size_t fact_len = 0, fact_cap = 0;
    static bool initialized = false;
    static idx_t current_k = 0;           /* the k whose factorial is in fact */

    /* initialization */
    if (!initialized) {
        terms_cap = 64;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));

        fact_cap = 64;
        fact = (unsigned char *)malloc(fact_cap * sizeof(unsigned char));
        fact[0] = 1;                     /* 0! = 1 */
        fact_len = 1;
        initialized = true;
    }

    /* extend terms until we have at least n of them */
    while (terms_len < n) {
        /* 1. digit sum of current fact */
        unsigned int digit_sum = 0;
        for (size_t i = 0; i < fact_len; ++i)
            digit_sum += fact[i];

        /* 2. test if digit_sum + 1 is prime */
        if (prime_small(digit_sum + 1))
            terms[terms_len++] = (seq_t)current_k;

        /* 3. advance to next factorial */
        ++current_k;                      /* new k */

        /* multiply fact by current_k (k! = (k-1)! * k) */
        unsigned long long carry = 0;
        for (size_t i = 0; i < fact_len; ++i) {
            unsigned long long prod = (unsigned long long)fact[i] * current_k + carry;
            fact[i] = (unsigned char)(prod % 10);
            carry    = prod / 10;
        }
        while (carry) {
            if (fact_len == fact_cap) {
                fact_cap *= 2;
                fact = (unsigned char *)realloc(fact, fact_cap * sizeof(unsigned char));
            }
            fact[fact_len++] = (unsigned char)(carry % 10);
            carry /= 10;
        }

        /* ensure terms array capacity */
        if (terms_len == terms_cap) {
            terms_cap *= 2;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
    }

    return terms[n - 1];
}
