/*
 * A195441  a(n) = denominator(Bernoulli_{n+1}(x) - Bernoulli_{n+1}).
 * https://oeis.org/A195441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of primes p ≤ (n+2)/(2+(n mod 2)) * for which the
 *            sum of the digits of (n+1) in base p is ≥ p
 * Range:     u8 n <= 19, u16 n <= 59, u32 n <= 179, u64 n <= 419,
 *            u128 n <= 1799, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ===========================================================================
 *  Prime generation (static, up to a fixed large limit).
 *  The bound with n ≤ 10000 never exceeds 10001, so a fixed limit of
 *  20000 is sufficient for all required calls.
 * ===========================================================================
 */
#define PRIME_LIMIT 20000

/* Prime array and its size (initialized lazily). */
static uint64_t *prime_arr = NULL;
static size_t prime_cnt  = 0;

/* Ensure that prime_arr is filled with all primes up to PRIME_LIMIT. */
static void prepare_primes(void)
{
    if (prime_arr != NULL) return;          /* already prepared */

    /* Simple sieve: allocate an array of bools. */
    uint8_t *is_composite = (uint8_t *)calloc(PRIME_LIMIT + 1, 1);
    if (!is_composite) abort();             /* allocation failure */

    /* Allocate space for primes (overallocate). */
    prime_arr = (uint64_t *)malloc(sizeof(uint64_t) * (PRIME_LIMIT + 1));
    if (!prime_arr) abort();

    prime_cnt = 0;
    for (uint64_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            prime_arr[prime_cnt++] = i;
            if (i <= PRIME_LIMIT / i) {
                for (uint64_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);
}

/* ===========================================================================
 *  Main sequence function.
 * ===========================================================================
 */
seq_t a(idx_t n)
{
    /* Prepare the list of primes once. */
    prepare_primes();

    /* Compute the bound (floor division). */
    idx_t bound = (n + 2) / (2 + (n & 1ULL));

    /* n+1 in the appropriate type. */
    seq_t N = (seq_t)n + (seq_t)1;

    seq_t prod = (seq_t)1;                 /* product accumulator */

    /* Iterate over primes up to the bound. */
    for (size_t i = 0; i < prime_cnt && prime_arr[i] <= bound; ++i) {
        uint64_t p = prime_arr[i];

        /* Sum of digits of N in base p. */
        seq_t temp = N;
        unsigned sum = 0;
        while (temp != (seq_t)0) {
            /* remainder < p (fits in unsigned). */
            sum += (unsigned)(temp % (seq_t)p);
            temp /= (seq_t)p;
        }

        if (sum >= p) {
            prod *= (seq_t)p;
        }
    }

    return prod;
}
