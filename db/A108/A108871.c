/*
 * A108871  Numbers n such that the number of digits required to write the
 *          prime factors of n is equal to the number of divisors of n.
 * https://oeis.org/A108871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: incremental search for numbers where the digit count of the
 * prime factorisation (with multiplicity) equals the number of divisors. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Auxiliary: number of decimal digits of a uint64_t */
static int count_digits(uint64_t v)
{
    int d = 0;
    do {
        v /= 10;
        ++d;
    } while (v != 0);
    return d;
}

/* Integer square root, with small range safety. */
static uint64_t isqrt_uint64(uint64_t x)
{
    uint64_t r = (uint64_t)sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Dynamic array for primes */
static uint64_t *primes = NULL;
static idx_t primes_cnt = 0;
static idx_t primes_cap = 0;
static uint64_t last_prime_val = 0;

static void add_prime(uint64_t p)
{
    if (primes_cnt == primes_cap) {
        primes_cap = primes_cap ? primes_cap * 2 : 16;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    primes[primes_cnt++] = p;
    last_prime_val = p;
}

/* Generate all primes up to at least 'limit' */
static void ensure_primes(uint64_t limit)
{
    if (primes_cnt == 0) {
        add_prime(2);
    }
    for (uint64_t cand = last_prime_val + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (idx_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) add_prime(cand);
    }
}

/* Dynamic array for terms */
static seq_t *terms = NULL;
static idx_t terms_cnt = 0;
static idx_t terms_cap = 0;

static void add_term(seq_t val)
{
    if (terms_cnt == terms_cap) {
        terms_cap = terms_cap ? terms_cap * 2 : 16;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_cnt++] = val;
}

/* Main function: returns the nth term of A108871 */
seq_t a(idx_t n)
{
    /* Generate terms up to n */
    static uint64_t current = 2;   /* last checked number (next to test) */
    while (terms_cnt < n) {
        /* Ensure primes are enough for factorisation of current */
        uint64_t lim = isqrt_uint64(current);
        ensure_primes(lim);

        uint64_t tmp = current;
        uint64_t tau = 1;
        int digits = 0;

        for (idx_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p * p > tmp) break;
            if (tmp % p == 0) {
                int e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }
                tau *= (uint64_t)(e + 1);
                digits += e * count_digits(p);
            }
        }
        if (tmp > 1) {          /* remaining prime factor */
            tau *= 2;
            digits += count_digits(tmp);
        }

        if (tau == (uint64_t)digits) {
            add_term((seq_t)current);
        }
        ++current;
    }
    return terms[n - 1];
}
