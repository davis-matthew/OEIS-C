/*
 * A078542  Unbalanced composite numbers.
 * https://oeis.org/A078542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 184, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A078542: Unbalanced composite numbers –
   Composite m such that sigma(m)/phi(m) is not an integer. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* Simple dynamic array for primes                                         */
/* ------------------------------------------------------------------------- */
static uint64_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

static void ensure_prime_capacity(size_t new_cap)
{
    if (new_cap <= primes_cap) return;
    size_t new_size = new_cap > 0 ? new_cap : 16;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_size * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = new_size;
}

static void add_prime(uint64_t p)
{
    ensure_prime_capacity(primes_len + 1);
    primes[primes_len++] = p;
}

/* ------------------------------------------------------------------------- */
/* Prime generation up to at least 'limit' (inclusive).             */
/* Uses trial division by the primes already known.                         */
/* ------------------------------------------------------------------------- */
static void ensure_primes_up_to(uint64_t limit)
{
    if (limit < 2) return;
    if (primes_len && primes[primes_len - 1] >= limit) return;

    uint64_t start = primes_len ? primes[primes_len - 1] + 1 : 2;
    for (uint64_t n = start; n <= limit; ++n) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            uint64_t p = primes[i];
            if (p * p > n) break;
            if (n % p == 0) { prime = false; break; }
        }
        if (prime) add_prime(n);
    }
}

/* ------------------------------------------------------------------------- */
/* Simple primality test using the prime list.                              */
/* ------------------------------------------------------------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    ensure_primes_up_to((uint64_t)(n + 1));
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/* Compute Euler's totient phi(n) and sum of divisors sigma(n).              */
/* Uses unsigned __int128 for intermediate values.                           */
/* ------------------------------------------------------------------------- */
static void compute_phi_sigma(uint64_t n, unsigned __int128 *phi, unsigned __int128 *sigma)
{
    unsigned __int128 res_phi = 1;
    unsigned __int128 res_sigma = 1;
    uint64_t temp = n;

    /* Ensure we have primes up to sqrt(n) (simpler to use n itself) */
    ensure_primes_up_to(temp);

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) { temp /= p; ++exp; }

            /* phi *= p^(exp-1) * (p-1) */
            unsigned __int128 p_pow = 1;
            for (int k = 0; k < exp - 1; ++k) p_pow *= p;
            res_phi *= p_pow * (p - 1);

            /* sigma *= (p^(exp+1) - 1) / (p-1) */
            unsigned __int128 p_pow2 = 1;
            for (int k = 0; k < exp + 1; ++k) p_pow2 *= p;
            res_sigma *= (p_pow2 - 1) / (p - 1);
        }
    }

    if (temp > 1) { /* remaining prime factor with exponent 1 */
        uint64_t p = temp;
        res_phi *= (p - 1);
        res_sigma *= (p + 1);
    }

    *phi = res_phi;
    *sigma = res_sigma;
}

/* ------------------------------------------------------------------------- */
/* Dynamic array for the generated sequence terms.                           */
/* ------------------------------------------------------------------------- */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

static void ensure_terms_capacity(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    size_t new_size = new_cap > 0 ? new_cap : 16;
    seq_t *tmp = (seq_t *)realloc(terms, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    terms = tmp;
    terms_cap = new_size;
}

/* Append a value to the sequence array.                                   */
static void append_term(seq_t val)
{
    ensure_terms_capacity(terms_len + 1);
    terms[terms_len++] = val;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function.                                                   */
/* ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;   /* undefined, safe return */

    /* Generate terms until we have the nth one. */
    static uint64_t last_checked = 1;    /* integer to examine next */

    while (terms_len < (size_t)n) {
        ++last_checked;
        if (is_prime_uint64(last_checked)) continue;   /* skip primes */

        unsigned __int128 phi, sigma;
        compute_phi_sigma(last_checked, &phi, &sigma);

        if (sigma % phi != 0) {           /* unbalanced composite */
            append_term((seq_t)last_checked);
        }
    }

    return terms[n - 1];
}
