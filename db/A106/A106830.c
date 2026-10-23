/*
 * A106830  Numerator of Sum_{ primes p <= n} 1/p.
 * https://oeis.org/A106830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of the sum of reciprocals of all primes p ≤ n,
 *            reduced to lowest terms
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 28, u64 n <= 52, u128 n <= 100,
 *            big any n
 * Helpers:   gcd_seq_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd_seq_internal.h"

/* Static state for memoisation and incremental computation */
static seq_t *numerators = NULL;          /* array of numerators for each n */
static size_t numerators_size = 0;        /* allocated capacity of numerators */

static uint64_t *prime_list = NULL;      /* list of primes discovered so far */
static size_t prime_cnt = 0;
static size_t prime_size = 0;

static seq_t current_num = 0;            /* numerator of the current sum (in lowest terms) */
static seq_t current_den = 1;            /* denominator of the current sum (in lowest terms) */
static idx_t last_n = 0;                 /* largest n that has been processed so far */

/* Extend all internal tables up to (and including) n. */
static void extend_to(idx_t n)
{
    if (n <= last_n)
        return;

    for (idx_t i = last_n + 1; i <= n; ++i) {
        /* --- primality test for i --- */
        bool is_prime = true;
        if (i < 2)
            is_prime = false;
        else {
            for (size_t k = 0; k < prime_cnt; ++k) {
                uint64_t p = prime_list[k];
                if (p > i / p) /* p*p would overflow or be > i */
                    break;
                if (i % p == 0) {
                    is_prime = false;
                    break;
                }
            }
        }

        /* --- update state if i is prime --- */
        if (is_prime) {
            /* grow the prime list if necessary */
            if (prime_cnt == prime_size) {
                size_t new_size = (prime_size == 0) ? 64 : prime_size << 1;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                    new_size * sizeof(uint64_t));
                prime_list = tmp;
                prime_size = new_size;
            }
            prime_list[prime_cnt++] = i;

            /* add 1/i to the current fraction: new_num/new_den = old + 1/i */
            seq_t p_seq = (seq_t)i;
            seq_t new_num = current_num * p_seq + current_den;
            seq_t new_den = current_den * p_seq;

            /* reduce to lowest terms */
            seq_t g = gcd_seq_internal(new_num, new_den);
            current_num = new_num / g;
            current_den = new_den / g;
        }
        /* --- store the numerator for this n --- */
        if ((size_t)i > numerators_size) {
            size_t new_sz = (numerators_size == 0) ? 64 : numerators_size << 1;
            while (new_sz < (size_t)i)
                new_sz <<= 1;
            seq_t *tmp = (seq_t *)realloc(numerators,
                                          new_sz * sizeof(seq_t));
            numerators = tmp;
            numerators_size = new_sz;
        }
        numerators[i - 1] = current_num;
    }

    last_n = n;
}

/* Entry point requested by the evaluator. */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined in OEIS, return 0 as a safe default */
        return (seq_t)0;
    extend_to(n);
    return numerators[n - 1];
}
