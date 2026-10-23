/*
 * A166747  Numbers with n digits from the set {1,2,4}, requiring a
 *          nondecreasing sequence of digits and a digits sum of A008578(n).
 * https://oeis.org/A166747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal number with n digits from {1,2,4},
 *            non‑decreasing, and digit sum = A008578(n). A008578(n) equals
 *            the (n‑1)th prime for n≥2 and 1 for n=1. The function returns
 *            the value as seq_t for any n≥1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define PRIME_LIMIT 200000   /* > 10000th prime (104729) */

static idx_t primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_built = false;

/* Build all primes up to PRIME_LIMIT using a simple sieve. */
static void build_primes(void)
{
    if (primes_built) return;

    bool *is_prime = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(1);
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;

    for (idx_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= PRIME_LIMIT; m += p)
                is_prime[m] = false;
        }
    }

    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (is_prime[i]) primes[prime_count++] = i;

    primes_built = true;
    free(is_prime);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Off‑setting: undefined below 1 */

    /* Digit sum needed: A008578(n) */
    seq_t sum;
    if (n == 1) {
        sum = (seq_t)1;
    } else {
        build_primes();
        /* For n≥2, sum = (n‑1)th prime (primes array is 0‑based) */
        if ((n - 2) >= prime_count) {
            /* Guard against unexpected large n; return 0 on error */
            return (seq_t)0;
        }
        sum = (seq_t)primes[n - 2];
    }

    /* Compute minimal number of 4’s (c4).  Derived from
       c4 = ceil((sum - 2n) / 2)  when (sum > 2n), else 0. */
    int64_t diff = (int64_t)sum - 2 * (int64_t)n;
    idx_t c4 = 0;
    if (diff > 0) c4 = (idx_t)((diff + 1) / 2);

    /* Upper bound for c4: c4 <= floor((sum - n)/3) */
    idx_t c4_max = (idx_t)(((seq_t)sum >= n) ? (((seq_t)sum - n) / 3) : 0);
    if (c4 > c4_max) c4 = c4_max;  /* Safety check */

    idx_t c2 = (idx_t)(((seq_t)sum - n) - 3 * (seq_t)c4);
    idx_t c1 = n - c2 - c4;

    /* Construct the number: c1 times '1', c2 times '2', c4 times '4'. */
    seq_t result = 0;
    for (idx_t i = 0; i < c1; ++i)
        result = result * (seq_t)10 + (seq_t)1;
    for (idx_t i = 0; i < c2; ++i)
        result = result * (seq_t)10 + (seq_t)2;
    for (idx_t i = 0; i < c4; ++i)
        result = result * (seq_t)10 + (seq_t)4;

    return result;
}
