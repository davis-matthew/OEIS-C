/*
 * A031919  a(n) = prime(10*n-8).
 * https://oeis.org/A031919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(10*n-8)
 * Range:     u8 n <= 6, u16 n <= 655, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* External linkage required by the build system */
seq_t a(idx_t n);

/* Static state for prime generation */
static bool *is_composite = NULL;    /* composite flag array (1‑based) */
static seq_t *prime_arr   = NULL;    /* array of primes, 0‑based */
static idx_t prime_cnt    = 0;        /* number of primes stored */
static idx_t sieve_limit  = 0;        /* highest number sieved */

/* Helper: extend the sieve so that at least 'needed' primes are available. */
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= needed)
        return;

    /* Upper bound for the N-th prime (valid for large N). */
    idx_t m = needed;
    double dm = (double)m;
    double upper;
    if (m < 6) {
        upper = 15.0;
    } else {
        upper = dm * (log(dm) + log(log(dm))) + 3.0;
    }
    idx_t limit = (idx_t)(upper) + 10;              /* safety margin */
    if (limit < 20) limit = 20;
    if (limit <= sieve_limit) limit = sieve_limit + 1000; /* increase only */

    /* Free previous sieve data */
    free(is_composite);
    free(prime_arr);

    /* Allocate new sieve */
    is_composite = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    prime_arr    = (seq_t*)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!is_composite || !prime_arr) {
        /* Allocation failed – zero state. The function will return 0. */
        prime_cnt = 0;
        sieve_limit = 0;
        return;
    }

    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            prime_arr[count++] = (seq_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[j] = true;
            }
        }
    }

    prime_cnt   = count;
    sieve_limit = limit;
}

/* Required function: returns the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Compute the requested prime index: 10*n-8. */
    idx_t prime_index = n * (idx_t)10 - (idx_t)8;
    if (prime_index == 0)
        return (seq_t)0; /* should not happen for n ≥ 1 */

    /* Ensure we have enough primes. */
    ensure_primes(prime_index);

    /* prime_arr is 0‑based, prime_index is 1‑based. */
    if (prime_index <= prime_cnt)
        return prime_arr[prime_index - 1];

    /* Should never reach here for valid n, but return 0 on failure. */
    return (seq_t)0;
}
