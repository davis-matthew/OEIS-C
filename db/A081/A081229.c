/*
 * A081229  a(n) is the sum of the common digits of n and the n-th prime in
 *          base 10, or -1 if there are no common digits.
 * https://oeis.org/A081229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A081229: a(n) = sum of distinct common digits of n and the n‑th prime;
   returns -1 if there is no common digit.  Primes are generated incrementally
   on the first request. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_PRIMES 50000          /* sufficient for n up to ≈10000 */
static uint32_t primes[MAX_PRIMES];
static size_t prime_cnt = 0;

/* Ensure that primes[0..n-1] contain at least n primes (1‑indexed). */
static void
ensure_primes_upto(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;
    uint32_t candidate = (prime_cnt == 0) ? 2u : primes[prime_cnt - 1] + 1u;

    while (prime_cnt < (size_t)n && prime_cnt < MAX_PRIMES) {
        bool is_prime = true;
        /* trial division by known primes up to sqrt(candidate) */
        size_t i = 0;
        for (; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
            primes[prime_cnt++] = candidate;
        candidate++;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)-1;   /* undefined below offset */

    ensure_primes_upto(n);
    if (prime_cnt < (size_t)n)  /* safety check */
        return (seq_t)-1;

    uint32_t prime = primes[(size_t)n - 1];   /* 1‑indexed prime */

    bool digitN[10] = {false};
    bool digitP[10] = {false};

    /* digits of n */
    idx_t tmp = n;
    while (tmp > 0) {
        int d = tmp % 10;
        digitN[d] = true;
        tmp /= 10;
    }

    /* digits of prime */
    uint32_t tp = prime;
    while (tp > 0) {
        int d = tp % 10;
        digitP[d] = true;
        tp /= 10;
    }

    seq_t sum = 0;
    bool found = false;
    for (int d = 0; d <= 9; ++d) {
        if (digitN[d] && digitP[d]) {
            sum += d;
            found = true;
        }
    }
    return found ? sum : (seq_t)-1;
}
