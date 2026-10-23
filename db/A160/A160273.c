/*
 * A160273  Successive differences (divided by 3) of the average of twin prime
 *          pairs divided by 2 (A040040).
 * https://oeis.org/A160273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p_{n+2} - p_{n+1}) / 6 where p_k is the k-th lower prime
 *            of a twin prime pair. The value is the successive difference
 *            (divided by 3) of (average of twin prime pairs)/2, as specified
 *            by OEIS A160273
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Helpers available from the build system */
#include "../utils/build_primes.h"

static uint64_t *twin_lower = NULL;   /* lower primes of twin pairs */
static size_t twin_cnt = 0;           /* number of stored twins */
static idx_t sieve_limit = 2000000;   /* current sieve limit */

/* Rebuild the twin list up to at least (required_n + 1) indices */
static void ensure_twins(idx_t required_n)
{
    if (twin_lower != NULL && twin_cnt > (size_t)required_n + 1)
        return; /* already enough */

    while (true) {
        /* Build primes up to current limit */
        idx_t *primes = NULL;
        size_t prime_cnt = 0;
        build_primes(sieve_limit, &primes, &prime_cnt);
        if (primes == NULL) exit(1); /* allocation failure */

        /* Count twin primes and collect the lower ones */
        size_t new_cnt = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i) {
            if (primes[i + 1] - primes[i] == 2)
                ++new_cnt;
        }

        uint64_t *new_arr = (uint64_t *)malloc(new_cnt * sizeof(uint64_t));
        if (new_arr == NULL) exit(1);

        size_t idx = 0;
        for (size_t i = 0; i + 1 < prime_cnt; ++i) {
            if (primes[i + 1] - primes[i] == 2)
                new_arr[idx++] = (uint64_t)primes[i];
        }

        /* Free old data */
        free(primes);
        free(twin_lower);

        twin_lower = new_arr;
        twin_cnt  = new_cnt;

        if (twin_cnt > (size_t)required_n + 1)
            break;                    /* enough twins obtained */

        /* Not enough twins: increase limit and try again */
        sieve_limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for n<1, but return 0 */

    /* Ensure we have at least (n+1) twin entries (0-based). */
    ensure_twins(n);

    /* Compute (twin_lower[n+1] - twin_lower[n]) / 6 */
    uint64_t diff = twin_lower[(size_t)n + 1] - twin_lower[(size_t)n];
    seq_t result = (seq_t)(diff / 6);
    return result;
}
