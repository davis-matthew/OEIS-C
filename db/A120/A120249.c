/*
 * A120249  Numerator of cfenc[n] (see definition in comments).
 * https://oeis.org/A120249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of cfenc[n] where cfenc[n] is the continued
 *            fraction * obtained by taking the sequence of (one plus the
 *            exponent of each prime * factor of n, including zero exponents
 *            for primes up to the largest prime * factor), and interpreting
 *            it as a continued fraction with the first term a0,...
 * Range:     u8 n <= 36, u16 n <= 82, u32 n <= 198, u64 n <= 478,
 *            u128 n <= 1102, big any n
 * Helpers:   get_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_exponent.h"

#define PRIME_LIMIT 2000000            /* enough for n ≈ 10 000 and larger tests */
#define MAX_PRIME_COUNT 150000         /* π(PRIME_LIMIT) ≈ 148933 */

static uint64_t primes[MAX_PRIME_COUNT];
static size_t primes_cnt = 0;
static bool primes_inited = false;

/* --- initialisation of the global prime list ---------------------------- */
static void init_primes(void)
{
    if (primes_inited)
        return;
    primes_inited = true;

    /* simple sieve of Eratosthenes */
    bool *is_composite = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_composite)
        return;   /* allocation failed – silence the compiler, results are undefined */

    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        is_composite[i] = false;
    is_composite[0] = is_composite[1] = true;

    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (!is_composite[p]) {
            for (size_t m = p * p; m <= PRIME_LIMIT; m += p)
                is_composite[m] = true;
        }
    }

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!is_composite[i])
            primes[primes_cnt++] = (uint64_t)i;

    free(is_composite);
}

/* --- main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n <= 1)                /* cfenc[1] = 1 by definition */
        return (seq_t)1;

    init_primes();

    /* factorise n into prime factors and exponents */
    uint64_t factor_primes[20];   /* 20 > number of distinct primes for n <= 10^7 */
    unsigned int factor_exps[20];
    size_t fc = 0;

    uint64_t rem = (uint64_t)n;
    for (size_t pi = 0; pi < primes_cnt && primes[pi] * primes[pi] <= rem; ++pi) {
        uint64_t p = primes[pi];
        if (rem % p == 0) {
            unsigned int cnt = 0;
            while (rem % p == 0) {
                rem /= p;
                ++cnt;
            }
            factor_primes[fc] = p;
            factor_exps[fc++] = cnt;
        }
    }
    if (rem > 1) {
        factor_primes[fc] = rem;
        factor_exps[fc] = 1;
        ++fc;
    }

    /* find index of the largest prime factor (must exist) */
    uint64_t largest = factor_primes[fc - 1];
    size_t last_idx = 0;
    for (size_t i = 0; i < primes_cnt && primes[i] <= largest; ++i)
        last_idx = i;

    /* start evaluation from the last term of the continued fraction */
    unsigned int term_last = get_exponent(primes[last_idx], factor_primes, factor_exps, fc) + 1;
    seq_t num = (seq_t)term_last;
    seq_t den = (seq_t)1;

    /* iterate backwards over the remaining primes */
    for (size_t i = last_idx; i-- > 0; ) {
        unsigned int exp = get_exponent(primes[i], factor_primes, factor_exps, fc);
        unsigned int term = exp + 1;

        seq_t tmp = num;               /* previous numerator (p) */
        num = (seq_t)term * num + den; /* new numerator: a*p + q */
        den = tmp;                     /* new denominator: p */
    }

    /* the returned value is the numerator of the continued fraction */
    return num;
}
