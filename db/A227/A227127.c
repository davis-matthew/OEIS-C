/*
 * A227127  The Akiyama-Tanigawa algorithm applied to 1/(1,2,3,5,... old prime
 *          numbers). Reduced numerators of the second row.
 * https://oeis.org/A227127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = reduced numerator of (n+1)*(1/p_n – 1/p_{n+1}) where p_0
 *            = 1 and p_k for k≥1 are the ordinary primes. Equivalent: a(n) =
 *            (n+1)*(p_{n+1}–p_n)/(p_n*p_{n+1}) reduced
 * Range:     u8 n <= 8, u16 n <= 54, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

#define MAX_PRIME_LIMIT 200000          /* enough for n up to ~10000 */

static bool primes_initialized = false;
static uint64_t primes_arr[MAX_PRIME_LIMIT + 2]; /* +2 for sentinel index */
static size_t primes_cnt = 0;

/* Simple sieve to fill primes_arr. p_0 is set to 1. */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* allocate temporary flag array */
    char *sieve = (char *)malloc((MAX_PRIME_LIMIT + 1) * sizeof(char));
    if (!sieve)
        return; /* allocation failure: leave primes uninitialized */

    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i)
        sieve[i] = 0;

    /* prime 1 is part of the special sequence */
    primes_arr[0] = 1;
    primes_cnt = 1;

    for (size_t p = 2; p <= MAX_PRIME_LIMIT; ++p) {
        if (!sieve[p]) {
            primes_arr[primes_cnt++] = p;
            if (p * p <= MAX_PRIME_LIMIT) {
                for (size_t q = p * p; q <= MAX_PRIME_LIMIT; q += p)
                    sieve[q] = 1;
            }
        }
    }

    free(sieve);
    primes_initialized = true;
}

seq_t a(idx_t n)
{
    init_primes();

    /* Ensure we have at least n+1 primes (p_{n+1}) */
    if ((size_t)(n + 1) >= primes_cnt) {
        /* In the unlikely case n exceeds the precomputed range,
           extend by simple trial increment (not expected for given limits). */
        size_t start = primes_cnt;
        uint64_t candidate = primes_arr[primes_cnt - 1] + 1;
        while ((size_t)(n + 1) >= primes_cnt) {
            bool is_prime = true;
            for (size_t i = 1; i * i <= candidate; ++i) {
                if (candidate % primes_arr[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime)
                primes_arr[primes_cnt++] = candidate;
            ++candidate;
        }
    }

    seq_t p_n   = (seq_t)primes_arr[(size_t)n];
    seq_t p_np1 = (seq_t)primes_arr[(size_t)(n + 1)];
    seq_t diff  = p_np1 - p_n;

    seq_t numerator = (seq_t)(n + 1) * diff;
    seq_t denominator = p_n * p_np1;

    seq_t g = gcd(numerator, denominator);
    return numerator / g;
}
