/*
 * A108144  Numbers n such that (n-1)/P(n-1) is a power of two > 1, where P(n)
 *          is the largest prime factor of n.
 * https://oeis.org/A108144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   compute the nth term of A108144 by testing integers n such that
 *            (n-1)/P(n-1) is a power of two > 1, where P(m) is the largest
 *            prime factor of m
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* internal storage for generated terms (1‑based) */
static seq_t *terms = NULL;
static size_t terms_sz  = 0;   /* number of stored terms   */
static size_t terms_cap = 0;   /* allocated capacity      */

/* prime sieve data (trial‑division generation) */
static uint64_t *primes      = NULL;
static size_t primes_sz      = 0;
static size_t primes_cap     = 0;
static uint64_t prime_next   = 2;   /* next integer to test for primality */

/* ensure primes array contains all primes <= limit */
static void ensure_primes_up_to(uint64_t limit)
{
    while (prime_next <= limit) {
        bool is_prime = true;
        uint64_t sq = (uint64_t)sqrt((double)prime_next);
        for (size_t i = 0; i < primes_sz && primes[i] <= sq; ++i) {
            if (prime_next % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (primes_sz == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) exit(1);
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_sz++] = prime_next;
        }
        ++prime_next;
    }
}

/* return the largest prime factor of n (0 if n <= 1) */
static seq_t largest_prime_factor_uint64(uint64_t n)
{
    if (n < 2) return 0;
    uint64_t limit = (uint64_t)sqrt((double)n) + 1;
    ensure_primes_up_to(limit);

    uint64_t tmp = n;
    seq_t largest = 0;
    for (size_t i = 0; i < primes_sz && primes[i]*primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            largest = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) largest = tmp;
    return largest;
}

/* generate terms until we have at least target terms */
static void generate_terms_upto(idx_t target)
{
    static idx_t next_n = 2;   /* next integer to test */
    while (terms_sz < target) {
        idx_t m = next_n - 1;
        if (m > 1) {
            seq_t largest = largest_prime_factor_uint64(m);
            if (largest > 0) {
                idx_t q = m / (idx_t)largest;   /* quotient */
                if (q > 1 && (q & (q - 1)) == 0) {
                    if (terms_sz == terms_cap) {
                        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                        if (!tmp) exit(1);
                        terms = tmp;
                        terms_cap = new_cap;
                    }
                    terms[terms_sz++] = (seq_t)next_n;
                }
            }
        }
        ++next_n;
    }
}

/* public function as required */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, but safe */
    if (n <= terms_sz) return terms[n-1];
    generate_terms_upto(n);
    return terms[n-1];
}
