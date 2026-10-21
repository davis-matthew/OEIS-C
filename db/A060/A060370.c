/*
 * A060370  Ratios (p-1)/d, where p is a prime and d is the number of digits
 *          of the periodic part of the decimal expansion of 1/p.
 * https://oeis.org/A060370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (p_n - 1) / d_n where p_n is the n-th prime and d_n is
 *            the length of the repeating part of 1/p_n in decimal
 *            representation
 * Range:     u8 n <= 54, u16 n <= 7362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _prime_limit_for_idx, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/_prime_limit_for_idx.h" /* size_t _prime_limit_for_idx(idx_t n); */

/* Array of primes generated as needed */
static idx_t *primeArray = NULL;      /* 0‑based array of primes */
static size_t primeCount = 0;         /* number of primes stored */

/* Ensure primeArray holds at least n primes (n ≥ 1) */
static void ensure_primes(idx_t n)
{
    if (primeCount >= (size_t)n)
        return;

    /* Estimate an upper bound for the n‑th prime */
    idx_t limit = (idx_t)_prime_limit_for_idx(n);
    if (limit < 10)
        limit = 10;        /* safety for very small n */

    for (;;) {
        size_t sz = (size_t)(limit + 1);
        uint8_t *isComp = (uint8_t*)calloc(sz, 1);
        if (!isComp)
            exit(EXIT_FAILURE);   /* out of memory */

        for (idx_t i = 2; i * i <= limit; ++i) {
            if (!isComp[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    isComp[j] = 1;
            }
        }

        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (!isComp[i])
                ++cnt;

        idx_t *arr = (idx_t*)malloc(cnt * sizeof(idx_t));
        if (!arr)
            exit(EXIT_FAILURE);

        size_t pos = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (!isComp[i])
                arr[pos++] = i;

        free(isComp);

        if (primeArray) free(primeArray);   /* free any previous array */
        primeArray = arr;
        primeCount = cnt;

        if (primeCount >= (size_t)n)
            break;      /* sufficient primes found */

        /* Increase limit and try again */
        limit *= 2;
    }
}

/* Compute the multiplicative order of 10 modulo prime p (p > 5). */
static seq_t order_of_10(idx_t p)
{
    if (p == 2 || p == 5)
        return (seq_t)1;      /* period length is 1 for 1/2 and 1/5 */

    idx_t phi   = p - 1;
    idx_t order = phi;
    idx_t temp  = phi;

    /* Reduce the order by prime factors of φ(p) */
    for (size_t i = 0; i < primeCount && primeArray[i] * primeArray[i] <= temp; ++i) {
        idx_t q = primeArray[i];
        if (temp % q == 0) {
            /* Remove all copies of q from temp */
            while (temp % q == 0)
                temp /= q;

            /* Try to divide order by q as many times as possible */
            while (order % q == 0) {
                seq_t test = mod_pow((seq_t)10,
                                    (uint64_t)(order / q),
                                    (seq_t)p);
                if (test == (seq_t)1)
                    order /= q;
                else
                    break;
            }
        }
    }

    /* Remaining prime factor (if any) */
    if (temp > 1) {
        idx_t q = temp;
        while (order % q == 0) {
            seq_t test = mod_pow((seq_t)10,
                                (uint64_t)(order / q),
                                (seq_t)p);
            if (test == (seq_t)1)
                order /= q;
            else
                break;
        }
    }

    return (seq_t)order;
}

/* Sequence a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1 */

    ensure_primes(n);              /* guarantee at least n primes */

    idx_t p   = primeArray[n - 1]; /* nth prime (1‑based) */
    seq_t ord = order_of_10(p);    /* period length of 1/p */
    seq_t result = ((seq_t)(p - 1)) / ord;

    return result;
}
