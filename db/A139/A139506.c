/*
 * A139506  Primes of the form x^2 + 26x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A139506 – primes of the form x^2 + 26xy + y^2 (x,y ≥ 0)
   The sequence consists of all primes p for which there exist
   non‑negative integers x,y with p = x^2 + 26xy + y^2.
   We generate the sequence by sieving primes up to a fixed limit
   and selecting those congruent to 1, 25 or 121 modulo 168,
   which is equivalent to the representability condition.
   The implementation precomputes the list once on first call and
   returns a(n) (1‑based) or 0 if n is larger than the precomputed
   range.  The code is valid C11/C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define PRECOMP_LIMIT 20000000ULL   /* 20 million – enough for n≈10000 */

static bool _seq_initialized = false;
static seq_t * _prime_seq = NULL;
static size_t _prime_seq_len = 0;

static void _init_sequence(void)
{
    if (_seq_initialized)
        return;

    /* sieve of Eratosthenes */
    uint8_t *is_prime = (uint8_t *)malloc((PRECOMP_LIMIT + 1) * sizeof(uint8_t));
    if (!is_prime)
        exit(EXIT_FAILURE);

    memset(is_prime, 1, PRECOMP_LIMIT + 1);
    is_prime[0] = 0;
    is_prime[1] = 0;

    for (uint64_t p = 2; p * p <= PRECOMP_LIMIT; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= PRECOMP_LIMIT; j += p)
                is_prime[j] = 0;
        }
    }

    /* count primes that satisfy the congruence */
    size_t count = 0;
    for (uint64_t v = 2; v <= PRECOMP_LIMIT; ++v) {
        if (!is_prime[v])
            continue;
        uint64_t m = v % 168ULL;
        if (m == 1ULL || m == 25ULL || m == 121ULL)
            ++count;
    }

    /* allocate and fill the array */
    _prime_seq = (seq_t *)malloc(count * sizeof(seq_t));
    if (!_prime_seq)
        exit(EXIT_FAILURE);

    size_t idx = 0;
    for (uint64_t v = 2; v <= PRECOMP_LIMIT; ++v) {
        if (!is_prime[v])
            continue;
        uint64_t m = v % 168ULL;
        if (m == 1ULL || m == 25ULL || m == 121ULL) {
            _prime_seq[idx++] = (seq_t)v;
        }
    }
    _prime_seq_len = count;

    free(is_prime);
    _seq_initialized = true;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;              /* undefined for n<1 */
    _init_sequence();
    if ((size_t)(n - 1) < _prime_seq_len)
        return _prime_seq[n - 1];
    return (seq_t)0;                  /* n beyond the precomputed range */
}
