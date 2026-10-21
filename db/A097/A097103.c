/*
 * A097103  Numbers m that are the hypotenuse of exactly 22 distinct
 *          integer-sided right triangles, i.e., m^2 can be written as a sum
 *          of two squares in 22 ways.
 * https://oeis.org/A097103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th positive integer m whose square m² has exactly
 *            22 distinct representations as a sum of two squares.
 *            Equivalently, the product over primes p ≡ 1 (mod 4) of (2·e_p +
 *            1) equals 45
 * Range:     u8 none, u16 n <= 100, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_realloc.h"          /* seq_realloc(seq_t*,size_t,size_t) */

static int primes[60000];
static size_t primes_cnt = 0;

/* Construct a small prime sieve up to 50000 once. */
static void init_primes(void)
{
    if (primes_cnt > 0)
        return;
    const int LIMIT = 50000;
    bool *is_prime = (bool *)malloc(LIMIT + 1);
    for (int i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
    for (int i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[primes_cnt++] = i;
    free(is_prime);
}

/* Cache of terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;
static uint64_t last_checked = 0;  /* largest m already examined */

/* Grow the terms array using seq_realloc, which works for both POD and
   non‑POD seq_t types. */
static void ensure_terms_cap(void)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 128;
        while (new_cap < terms_len + 64)
            new_cap <<= 1;
        terms = seq_realloc(terms, terms_cap, new_cap);
        terms_cap = new_cap;
    }
}

/* Return true iff m has exactly 22 distinct representations of m² as a²+b²
   with a,b>0.  The number of such representations is (prod-1)/2 where
   prod = ∏_{p≡1(4)}(2·e_p+1).  Hence prod must equal 45. */
static bool has_22_triangles(uint64_t m)
{
    uint64_t temp = m;
    uint64_t prod  = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > temp)
            break;
        if (temp % p == 0) {
            unsigned exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            if (p % 4 == 1) {
                prod *= (2 * exp + 1);
                if (prod > 45)       /* early exit */
                    return false;
            }
        }
    }

    if (prod > 45)
        return false;

    if (temp > 1) {                 /* remaining prime factor > √m */
        if (temp % 4 == 1) {
            prod *= 3;              /* exponent 1 contributes factor 3 */
            if (prod > 45)
                return false;
        }
    }

    return prod == 45;
}

/* Generate terms until at least n terms are stored. */
static void generate_upto(idx_t n)
{
    init_primes();
    while (terms_len < (size_t)n) {
        ++last_checked;
        if (has_22_triangles(last_checked)) {
            ensure_terms_cap();
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
}

/* Public entry point.  a(1) is the first term listed in the OEIS entry. */
seq_t a(idx_t n)
{
    if (n == 0)           /* undefined before offset */
        return (seq_t)0;
    generate_upto(n);
    return terms[n - 1];
}
