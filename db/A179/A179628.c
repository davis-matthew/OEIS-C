/*
 * A179628  Sum of the numbers already killed in the first jump of a Sieve of
 *          Eratosthenes table.
 * https://oeis.org/A179628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=prime(n)+1}^{2*prime(n)-1} k , summed only over the
 *            composite k (i.e. k is not prime). This sums the composite
 *            numbers in the first optimal “jump’’ of the Sieve of
 *            Eratosthenes starting at the n‑th prime
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- static state for the sieve ------------------------------------------------ */
static size_t sieve_limit = 0;          /* current limit of the sieve               */
static uint8_t *is_composite = NULL;   /* flag array: 0 → prime, 1 → composite   */
static idx_t *prime_arr   = NULL;      /* list of primes (0‑based index)           */
static size_t prime_cnt   = 0;          /* number of primes stored                 */

/* enlarge or create the sieve up to at least 'limit' */
static void ensure_sieve(size_t limit)
{
    if (limit <= sieve_limit) return;

    free(is_composite);
    free(prime_arr);

    sieve_limit = limit;
    is_composite = (uint8_t *)calloc(sieve_limit + 1, sizeof(uint8_t));
    if (!is_composite) return; /* out of memory – caller should not crash */

    /* initialise 0 and 1 as composite */
    is_composite[0] = is_composite[1] = 1;

    /* simple Eratosthenes sieve */
    for (size_t i = 2; i * i <= sieve_limit; ++i) {
        if (!is_composite[i]) {
            for (size_t j = i * i; j <= sieve_limit; j += i) {
                is_composite[j] = 1;
            }
        }
    }

    /* count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= sieve_limit; ++i) {
        if (!is_composite[i]) ++cnt;
    }

    prime_arr = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!prime_arr) { prime_cnt = 0; return; }
    prime_cnt = cnt;

    size_t idx = 0;
    for (size_t i = 2; i <= sieve_limit; ++i) {
        if (!is_composite[i]) prime_arr[idx++] = (idx_t)i;
    }
}

/* ------------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(0) = 0 by definition (the range [2,3] contains no composites). */
    if (n == 0) return 0;

    /* bootstrap the sieve to a modest size.  This is more than enough for all
       requested n (up to 10000).  It will grow automatically if needed. */
    if (sieve_limit == 0) ensure_sieve(200000);

    /* ensure we have at least n+1 primes in the list */
    while (prime_cnt <= (size_t)n) {
        size_t new_limit = sieve_limit * 2 + 1;
        if (new_limit < 200000) new_limit = 200000;
        ensure_sieve(new_limit);
    }

    /* get the desired prime */
    idx_t p = prime_arr[(size_t)n];

    /* guarantee the sieve covers up to 2*p-1 */
    size_t needed = (size_t)(p * 2 - 1);
    if (needed > sieve_limit) ensure_sieve(needed);

    /* accumulate the sum of composites in the requested interval */
    seq_t sum = 0;
    for (uint64_t val = (uint64_t)p + 1; val <= (uint64_t)(2 * p - 1); ++val) {
        if (is_composite[(size_t)val]) {
            sum += (seq_t)val;
        }
    }
    return sum;
}
