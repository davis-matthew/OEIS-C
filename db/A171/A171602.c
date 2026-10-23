/*
 * A171602  Lesser of two consecutive primes such that half-sums are
 *          semiprimes.
 * https://oeis.org/A171602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the lower of two consecutive primes p_k, p_{k+1} such
 *            that (p_k + p_{k+1}) / 2 is a semiprime (product of exactly two
 *            (not necessarily distinct) primes)
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* The sequence type and index type are supplied by the build. */
static seq_t *terms = NULL;          /* stored results, a(1) at index 0 */
static size_t terms_len = 0;
static size_t terms_cap = 0;

static uint64_t *primes = NULL;      /* primes up to current maximum */
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Ensure the primes array has space for at least one more prime. */
static void grow_primes(void)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint64_t *new_arr = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!new_arr) exit(1);   /* out of memory */
        primes = new_arr;
        primes_cap = new_cap;
    }
}

/* Ensure the terms array has space for at least one more term. */
static void grow_terms(void)
{
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) exit(1);   /* out of memory */
        terms = new_arr;
        terms_cap = new_cap;
    }
}

/* Return true if x is semiprime: exactly two prime factors counting multiplicity. */
static bool is_semiprime_uint64(uint64_t x)
{
    if (x < 4) return false;          /* first semiprime is 4 = 2*2 */
    uint64_t temp = x;
    int factors = 0;

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > temp) break;
        while (temp % p == 0) {
            ++factors;
            if (factors > 2) return false;
            temp /= p;
        }
    }
    if (temp > 1) ++factors;          /* remaining prime factor */
    return factors == 2;
}

/* Test if cand is prime using the current list of primes. */
static bool is_prime_trivial(uint64_t cand)
{
    if (cand < 2) return false;
    if (cand == 2) return true;
    if (cand % 2 == 0) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* Generate primes and sequence terms until at least n terms are known. */
static void ensure_terms(idx_t n)
{
    /* Initialize primes if not yet done. */
    if (primes_len == 0) {
        grow_primes();
        primes[primes_len++] = 2;
        grow_primes();
        primes[primes_len++] = 3;
    }

    /* Extend until we have enough terms. */
    while (terms_len < n) {
        /* Find next prime after the current last prime. */
        uint64_t cand = primes[primes_len - 1] + 2;  /* skip even numbers */
        while (!is_prime_trivial(cand)) cand += 2;

        grow_primes();
        primes[primes_len++] = cand;

        /* For each new prime after the first, examine the pair with its predecessor. */
        if (primes_len >= 2) {
            uint64_t prev = primes[primes_len - 2];
            uint64_t curr = primes[primes_len - 1];
            uint64_t avg = (prev + curr) >> 1;   /* integer division by 2 */

            if (is_semiprime_uint64(avg)) {
                grow_terms();
                terms[terms_len++] = (seq_t)prev;   /* store lower member of the pair */
            }
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but return 0 */
    ensure_terms(n);
    return terms[n - 1];
}
