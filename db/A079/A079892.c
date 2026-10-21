/*
 * A079892  Least number > n having one more distinct prime factor than n.
 * https://oeis.org/A079892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 209, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes a(n) = the smallest integer > n whose number of
   distinct prime factors ω(m) satisfies ω(m) = ω(n) + 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Simple prime sieve up to a fixed limit. */
static uint32_t *primes = NULL;      /* list of primes                      */
static size_t   primes_cnt = 0;      /* number of primes stored             */

/* Initialise the prime table (once) */
static void init_primes(void)
{
    if (primes != NULL)
        return;

    /* A generous upper bound that is sufficient for n up to 10 000 */
    const uint32_t limit = 100000;
    size_t cap = limit / 10 + 10;     /* initial capacity estimate */

    primes = (uint32_t *)malloc(cap * sizeof(uint32_t));
    if (primes == NULL)
        exit(1); /* fatal */

    primes[0] = 2;
    primes_cnt = 1;

    for (uint32_t num = 3; num <= limit; num += 2) {
        bool prime = true;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint32_t p = primes[i];
            if ((seq_t)p * (seq_t)p > num)
                break;
            if (num % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (primes_cnt == cap) {
                cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, cap * sizeof(uint32_t));
                if (tmp == NULL)
                    exit(1);
                primes = tmp;
            }
            primes[primes_cnt++] = num;
        }
    }
}

/* Count distinct prime factors using the prepared prime list. */
static inline int count_distinct_primes(seq_t x)
{
    if (x <= 1)
        return 0;

    int cnt = 0;
    seq_t rest = x;
    for (size_t i = 0; i < primes_cnt &&
                     (seq_t)primes[i] * (seq_t)primes[i] <= rest; ++i) {
        seq_t p = primes[i];
        if (rest % p == 0) {
            ++cnt;
            while (rest % p == 0)
                rest /= p;
        }
    }
    if (rest > 1)
        ++cnt;   /* remaining factor > sqrt(x) is prime */
    return cnt;
}

/* Main function for OEIS evaluation. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* behaviour below offset is irrelevant */

    init_primes();

    seq_t n_seq = (seq_t)n;
    int omega_n = count_distinct_primes(n_seq);

    seq_t candidate = n_seq + 1;
    while (1) {
        int omega_c = count_distinct_primes(candidate);
        if (omega_c == omega_n + 1)
            return candidate;
        ++candidate;
    }
}
