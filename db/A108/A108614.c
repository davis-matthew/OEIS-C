/*
 * A108614  Semiprimes with non-semiprimes digits (no digits 4,6,9 in
 *          semiprimes).
 * https://oeis.org/A108614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digits_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A108614: 
   enumerate positive integers, keep those that are semiprimes
   (product of exactly two primes, counting multiplicity) and that
   contain none of the digits 4, 6, or 9.  The function is memoised
   so that calls in any order are efficient. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digits_allowed.h"

static seq_t *values    = NULL; /* cached sequence values */
static idx_t  values_cap = 0, values_len = 0;
static idx_t  next_candidate = 4;        /* next integer to test */

/* A sieve of primes up to MAX_SIEVE (≥ sqrt of the largest candidate will be tested). */
#define MAX_SIEVE 2000000
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;
static bool     sieve_done = false;
static unsigned char *isComposite = NULL;

static void init_sieve(void)
{
    if (sieve_done) return;

    isComposite = (unsigned char*)calloc(MAX_SIEVE + 1, 1);
    if (!isComposite) exit(1);

    primes = (uint64_t*)malloc((MAX_SIEVE / 10 + 10) * sizeof(uint64_t));
    if (!primes) exit(1);

    for (int i = 2; i <= MAX_SIEVE; ++i) {
        if (!isComposite[i]) {
            primes[primes_cnt++] = (uint64_t)i;
            if ((int64_t)i * i <= MAX_SIEVE) {
                for (int j = i * i; j <= MAX_SIEVE; j += i)
                    isComposite[j] = 1;
            }
        }
    }
    sieve_done = true;
}

/* true iff x is a semiprime: product of exactly two primes (counting multiplicity) */
static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;          /* smallest semiprime is 4 */
    size_t cnt = 0;
    for (size_t i = 0; i < primes_cnt;
         ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        while (x % p == 0) {
            ++cnt;
            if (cnt > 2) return false;
            x /= p;
        }
    }
    if (x > 1) {            /* remaining prime factor */
        ++cnt;
        if (cnt > 2) return false;
    }
    return cnt == 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* undefined, but return 0 */

    init_sieve();

    /* extend cache until it contains at least n values */
    while (values_len < n) {
        /* skip numbers that contain forbidden digits */
        while (!digits_allowed((uint64_t)next_candidate)) ++next_candidate;

        if (is_semiprime((uint64_t)next_candidate)) {
            /* grow cache if needed */
            if (values_len == values_cap) {
                size_t new_cap = values_cap ? values_cap * 2 : 4;
                seq_t *tmp = (seq_t*)realloc(values, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                values = tmp;
                values_cap = new_cap;
            }
            values[values_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }

    return values[n - 1];
}
