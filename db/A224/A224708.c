/*
 * A224708  The number of unordered partitions {a,b} of n such that a and b
 *          are composite.
 * https://oeis.org/A224708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 689, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224708
 * a(n) = the number of unordered pairs {a,b} such that a+b = n and
 *        both a and b are composite numbers (i.e. >=4 and not prime).
 * The implementation uses a sieve of Eratosthenes to test compositeness
 * and counts pairs by iterating over all b from 1 to floor(n/2).
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sieve */
static bool *is_prime = NULL;      /* is_prime[i] == true  <=>  i is prime   */
static idx_t prime_limit = 0;     /* current maximum n sieved for */

/* initialise or extend the prime sieve up to 'limit' */
static void init_prime_sieve(idx_t limit)
{
    if (limit <= prime_limit)
        return;       /* already sufficient */

    /* free previous allocation if any */
    if (is_prime != NULL) {
        free(is_prime);
    }

    /* allocate new sieve array, size limit+1 to include 'limit' itself */
    size_t sz = (size_t)(limit + 1);
    is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime)
        exit(1);      /* allocation failed */

    /* default all numbers to prime, then apply sieve */
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[i] = true;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    prime_limit = limit;
}

/* Function required by the build system */
seq_t a(idx_t n)
{
    seq_t count = 0;

    if (n < 8)          /* no composite partitions are possible */
        return 0;

    /* ensure we can test primality for all numbers up to n */
    init_prime_sieve(n);

    for (idx_t b = 1; b <= n / 2; ++b) {
        idx_t a = n - b;

        /* both a and b must be composite: >=4 and not prime */
        if (b >= 4 && !is_prime[b] && a >= 4 && !is_prime[a]) {
            ++count;
        }
    }

    return count;
}
