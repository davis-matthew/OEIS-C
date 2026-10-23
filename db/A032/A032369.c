/*
 * A032369  Hall normalization constants.
 * https://oeis.org/A032369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator of 2^(n^2-n) * Gamma(n^2/2) / (Pi^(n/2) *
 *            Product_{i=1..n+1} Gamma(i))
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   exponent_d_in_product, seq_from_str, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/exponent_d_in_product.h"

static uint8_t* composite = NULL;          /* 0 = prime, 1 = composite */
static int* primes = NULL;                 /* list of primes */
static size_t primes_count = 0;
static size_t primes_capacity = 0;
static size_t composite_limit = 0;         /* largest limit already sieved */

/* Sieve of Eratosthenes up to 'limit' (inclusive).  Rebuilds data if needed. */
static void sieve_up_to(idx_t limit)
{
    if (limit <= composite_limit) return;

    /* free old data */
    free(composite);
    free(primes);
    composite = (uint8_t*)malloc((size_t)(limit + 1));
    if (!composite) abort();
    memset(composite, 0, (size_t)(limit + 1));

    composite_limit = (size_t)limit;

    /* mark composites */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                composite[j] = 1;
        }
    }

    /* collect primes */
    primes_count = 0;
    primes_capacity = 16;
    primes = (int*)malloc(primes_capacity * sizeof(int));
    if (!primes) abort();

    for (idx_t i = 2; i <= limit; ++i) {
        if (!composite[i]) {
            if (primes_count == primes_capacity) {
                primes_capacity <<= 1;
                primes = (int*)realloc(primes, primes_capacity * sizeof(int));
                if (!primes) abort();
            }
            primes[primes_count++] = (int)i;
        }
    }
}

/* Exponent of prime 'p' in 'n!' (Legendre's formula). */
static inline idx_t legendre_factorial(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset 1, but defined for safety */

    const idx_t m = n * (n - 1);          /* exponent of 2 in numerator */

    const int is_even = (n & 1) == 0;
    idx_t fact_num;              /* factorial argument for numerator */
    idx_t fact_den = 0;          /* factorial argument for denominator (odd case) */
    idx_t k = 0;

    if (is_even) {
        fact_num = n * n / 2 - 1;      /* (n^2/2) - 1 */
    } else {
        k        = (n * n - 1) / 2;    /* integer k = (n^2-1)/2 */
        fact_num = n * n - 1;          /* 2*k = n^2 - 1 */
        fact_den = k;                  /* k! in denominator */
    }

    /* Prime limit needed for numerator factorial (or 2*k) and n itself */
    idx_t prime_limit = fact_num;
    if (prime_limit < n) prime_limit = n;

    sieve_up_to(prime_limit);

    seq_t result = seq_from_str("1");

    for (size_t idx = 0; idx < primes_count; ++idx) {
        int p = primes[idx];
        if ((idx_t)p > prime_limit) break;

        idx_t exp_d = exponent_d_in_product((idx_t)p, n);

        idx_t exp_num = 0;
        if (is_even) {
            exp_num = legendre_factorial(fact_num, (idx_t)p);
        } else {
            exp_num = legendre_factorial(fact_num, (idx_t)p) -
                      legendre_factorial(fact_den,  (idx_t)p);
            if (p == 2) {
                exp_num -= 2 * k;   /* division by 4^k */
            }
        }

        if (p == 2) {
            exp_num += m;           /* additional factor 2^(n^2-n) */
        }

        if (exp_num > exp_d) {
            idx_t exp = exp_num - exp_d;
            seq_t pow_part = seq_pow((seq_t)p, exp);
            result = seq_mul(result, pow_part);
        }
        /* exp_num <= exp_d would imply a non‑integer result, which should not occur */
    }

    return result;
}
