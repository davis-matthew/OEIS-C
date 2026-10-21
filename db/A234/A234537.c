/*
 * A234537  Number of nontrivial non-Goldbach partitions of 2n into two odd
 *          parts (with smaller part greater than 1).
 * https://oeis.org/A234537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = ceil(n/2)-1 - Sum_{i=3..n} A010051(i)*A010051(2*n-i)
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         where A010051(i) = 1 if i is prime, 0 otherwise */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static sieve table up to the current maximum 2*n */
static uint8_t *prime_flag = NULL;   /* 1 = prime candidate, 0 = composite */
static idx_t  prime_limit  = 0;       /* largest index currently stored */

/* Build (or extend) a simple Eratosthenes sieve up to 2*n */
static void ensure_sieve(idx_t n)
{
    idx_t limit = 2 * n;
    if (limit <= prime_limit)
        return;          /* already sufficient */

    /* grow array */
    uint8_t *new_flag = (uint8_t*)realloc(prime_flag,
                                         sizeof(uint8_t) * (limit + 1));
    if (!new_flag) {
        /* allocation failure – terminate program */
        exit(EXIT_FAILURE);
    }
    prime_flag = new_flag;

    /* initialise all entries to "prime candidate" */
    for (idx_t i = 0; i <= limit; ++i)
        prime_flag[i] = 1;
    if (limit >= 0) prime_flag[0] = 0;
    if (limit >= 1) prime_flag[1] = 0;

    /* sieve */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (prime_flag[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                prime_flag[k] = 0;
        }
    }

    prime_limit = limit;
}

/* OEIS sequence A234537 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined value for the offset 1 */

    /* ensure we can test primality up to 2*n */
    ensure_sieve(n);

    /* count Goldbach partitions of 2*n with both primes in [3..n] */
    seq_t goldbach = 0;
    for (idx_t i = 3; i <= n; ++i) {
        if (prime_flag[i] && prime_flag[2 * n - i])
            goldbach += 1;
    }

    /* total number of partitions of 2*n into two odd parts with smaller part >1 */
    seq_t total = ((seq_t)(n + 1) / 2) - 1;

    /* desired value */
    return total - goldbach;
}
