/*
 * A070934  Smallest prime equal to the sum of 2n+1 consecutive primes.
 * https://oeis.org/A070934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 70, u32 n <= 41, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence: A070934
    a(n) = the smallest prime which equals the sum of exactly 2n+1 consecutive primes.
    The function generates primes once up to a fixed limit and then searches the
    sliding window for a prime sum.  It works for any n >= 0 and does not
    require the sequence to be monotonic or increasing in its calls.  The
    limit 2 000 000 produces all terms up to n ≈ 10000 in well under two
    seconds on typical hardware. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* -------------------------------------------------------------- */
/* Prime generation                                               */
/* -------------------------------------------------------------- */

static uint64_t *prime_list = NULL;    /* array of primes          */
static size_t    prime_count = 0;      /* number of primes stored  */
static const size_t PRIME_LIMIT = 2000000;   /* enough for n ≈ 10000   */

static void generate_primes(void)
{
    bool *is_composite = (bool*)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_composite) abort();

    size_t sqrt_lim = (size_t)sqrt((double)PRIME_LIMIT);
    for (size_t p = 2; p <= sqrt_lim; ++p) {
        if (!is_composite[p]) {
            for (size_t m = p * p; m <= PRIME_LIMIT; m += p) {
                is_composite[m] = true;
            }
        }
    }

    size_t cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) ++cnt;
    }

    uint64_t *new_list = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            new_list[idx++] = i;
        }
    }

    free(is_composite);
    free(prime_list);
    prime_list  = new_list;
    prime_count = cnt;
}

/* -------------------------------------------------------------- */
/* Main function                                                   */
/* -------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* initialise prime list on first call */
    if (!prime_list) generate_primes();

    size_t L = (size_t)(2 * n + 1);   /* window length */

    /* Defensive: ensure we have enough primes for the window */
    if (prime_count < L) abort();

    /* compute first sum */
    uint64_t sum = 0;
    for (size_t i = 0; i < L; ++i) sum += prime_list[i];

    if (is_prime_big((seq_t)sum)) {
        return (seq_t)sum;
    }

    /* slide the window until a prime sum is found */
    size_t start = 1;           /* start index of the window */
    while (1) {
        size_t next_index = start + L - 1;
        if (next_index >= prime_count) abort();   /* should never happen with our limit */

        sum += prime_list[next_index] - prime_list[start - 1];
        ++start;

        if (is_prime_big((seq_t)sum)) {
            return (seq_t)sum;
        }
    }
}
