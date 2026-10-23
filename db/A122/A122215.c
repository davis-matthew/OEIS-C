/*
 * A122215  Denominators in infinite products for Pi/2, e and e^gamma
 *          (reduced).
 * https://oeis.org/A122215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator(Product_{k=1..n}
 *            k^((-1)^k*binomial(n-1,k-1)))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Utility headers  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
/*  The helper functions supplied by the build system  */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/*  Cached data used by all calls  */
static uint64_t *global_primes = NULL;          /* primes <= max_sieved */
static size_t     global_prime_cnt = 0;
static idx_t      global_max_sieved = 0;         /* last limit up to which primes were generated */
static int        *prime_index_map = NULL;      /* map prime value -> index in global_primes array */
static size_t     prime_index_map_size = 0;      /* size of prime_index_map */

/*  Largest number of primes up to 10000  */
#define MAX_PRIME_COUNT 2000
static seq_t even_sum[MAX_PRIME_COUNT];
static seq_t odd_sum[MAX_PRIME_COUNT];

/*  Helper: recompute prime list and index map up to at least n  */
static void ensure_prime_cache(idx_t n)
{
    if (n <= global_max_sieved)
        return;

    /* free previous cache */
    if (global_primes) free(global_primes);
    if (prime_index_map) free(prime_index_map);

    /* build primes up to n */
    build_primes_upto(n, &global_primes, &global_prime_cnt);
    global_max_sieved = n;

    /* allocate index map of size n+1, initialized to -1 */
    prime_index_map_size = n + 1;
    prime_index_map = (int*)malloc(prime_index_map_size * sizeof(int));
    if (!prime_index_map) exit(EXIT_FAILURE);
    for (size_t i = 0; i < prime_index_map_size; ++i)
        prime_index_map[i] = -1;
    for (size_t i = 0; i < global_prime_cnt; ++i)
        prime_index_map[global_primes[i]] = (int)i;
}

/*  Main function  */
seq_t a(idx_t n)
{
    /* Offset: a(1)=1, a(2)=1 */
    if (n <= 2)
        return (seq_t)1;

    /* Ensure we have all primes up to n */
    ensure_prime_cache(n);

    /* Count primes <= n */
    size_t prime_count = 0;
    for (; prime_count < global_prime_cnt && global_primes[prime_count] <= (uint64_t)n; ++prime_count) {}

    /* Zero the sum arrays for the primes we will use */
    for (size_t i = 0; i < prime_count; ++i) {
        even_sum[i] = 0;
        odd_sum[i] = 0;
    }

    /* Binomial coefficient C(n-1, k-1), starting with k=1 */
    seq_t C = 1;      /* C = binomial(n-1, 0) */

    /* Temporary storage for factorization of k */
    uint64_t pf[64];
    uint64_t ep[64];
    size_t pf_cnt;

    for (idx_t k = 1; k <= n; ++k) {
        /* factorize k */
        factor_u64((uint64_t)k, pf, ep, &pf_cnt);

        /* add contributions for each prime factor of k */
        for (size_t j = 0; j < pf_cnt; ++j) {
            uint64_t prime_val = pf[j];
            int idx = prime_index_map[(int)prime_val];
            if (idx < 0)
                continue;                 /* prime > n, never happens here */
            seq_t mult = C * (seq_t)ep[j];
            if (k & 1U)      /* odd k -> negative exponent */
                odd_sum[idx] += mult;
            else
                even_sum[idx] += mult;
        }

        /* update binomial coefficient for next k (k+1) */
        if (k < n) {
            seq_t num = C * (seq_t)(n - k);
            C = num / (seq_t)k;          /* exact division */
        }
    }

    /* Assemble the result: product over primes where odd_sum > even_sum */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_count; ++i) {
        if (odd_sum[i] > even_sum[i]) {
            seq_t exp = odd_sum[i] - even_sum[i];
            seq_t pow = big_pow((seq_t)global_primes[i], exp);
            result = result * pow;
        }
    }

    return result;
}
