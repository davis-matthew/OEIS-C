/*
 * A094338  (Sum of composites among next n numbers)-(sum of primes among next
 *          n numbers).
 * https://oeis.org/A094338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k=L}^{R} (isComposite(k) ? k : 0) – Σ_{k=L}^{R}
 *            (isPrime(k) ? k : 0) with L = n*(n-1)/2 + 1, R = n*(n+1)/2. The
 *            natural numbers are partitioned into blocks of lengths 1,2,3,… .
 *            The function builds a sieve of Eratosthenes on demand and
 *            evaluates each block in O(n)
 * Range:     i8 n <= 6, i16 n <= 45, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool *prime_flag = NULL;      /* true if index is prime */
static size_t sieve_limit = 0;       /* highest number sieved so far */

/* Build a sieve of erathostenes up to N (inclusive). */
static void build_sieve(size_t N)
{
    if (prime_flag) {
        free(prime_flag);
    }
    prime_flag = (bool *)malloc((N + 1) * sizeof(bool));
    if (!prime_flag) abort();                /* memory failure */
    memset(prime_flag, true, (N + 1) * sizeof(bool));
    if (N >= 0) prime_flag[0] = false;
    if (N >= 1) prime_flag[1] = false;

    for (size_t i = 2; i * i <= N; ++i) {
        if (prime_flag[i]) {
            for (size_t j = i * i; j <= N; j += i) {
                prime_flag[j] = false;
            }
        }
    }
    sieve_limit = N;
}

/* Return a(n) for the OEIS sequence A094338. */
seq_t a(idx_t n)
{
    if ((idx_t)0 == n) return 0;          /* not defined for n=0, return 0 */

    uint64_t n64 = (uint64_t)n;
    uint64_t R = n64 * (n64 + 1) / 2;     /* upper bound of the block */
    uint64_t L = n64 * (n64 - 1) / 2 + 1; /* lower bound of the block */

    if (R > sieve_limit) {
        build_sieve((size_t)R);
    }

    seq_t comp = 0;
    seq_t primeSum = 0;
    for (uint64_t k = L; k <= R; ++k) {
        if (k <= 1) continue;            /* 1 is neither prime nor composite */
        if (prime_flag[(size_t)k]) {
            primeSum += (seq_t)k;
        } else {
            comp += (seq_t)k;
        }
    }
    return comp - primeSum;
}
