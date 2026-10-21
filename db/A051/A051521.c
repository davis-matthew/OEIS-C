/*
 * A051521  Number of ways to represent n as k/d(k), where d(k) = A000005(k)
 *          is the number of divisors of k.
 * https://oeis.org/A051521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k : k/d(k) == n }|, where d(k) = number of divisors of
 *            k
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count.h"

/* Max n expected in the tests (can be increased safely) */
#define MAX_N 10000
/* Upper bound for prime sieve: 2*n for the maximal n */
#define MAX_SIEVE (2 * MAX_N)

/* Storage for prime numbers up to MAX_SIEVE (there are 2262 primes <=20000) */
static uint64_t primes[2300];
static int primes_cnt;

/* Prime sieve up to limit (max 2*MAX_N) */
static void generate_primes(uint64_t limit)
{
    static bool comp[MAX_SIEVE + 1];
    for (uint64_t i = 0; i <= limit; ++i) comp[i] = false;

    primes_cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!comp[i]) {
            primes[primes_cnt++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    comp[j] = true;
            }
        }
    }
}

/* Sequence function */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    /* Ensure primes up to 2*N are available */
    if (N > MAX_N) {
        generate_primes(2 * N);  /* enlarge if needed */
    } else {
        generate_primes(2 * N);
    }

    seq_t count = 0;
    uint64_t max_t = 4 * N;      /* from inequality t <= 4n   */
    for (uint64_t t = 1; t <= max_t; ++t) {
        uint64_t k = N * t;      /* k = n * d(k) candidate   */
        uint64_t d_k = divisor_count(k, primes, primes_cnt);
        if (d_k == t) ++count;
    }
    return count;
}
