/*
 * A067773  a(n) is the unique positive integer m which has a self-conjugate
 *          partition whose parts are the first n primes.
 * https://oeis.org/A067773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*prime(n) + \sum_{i=1}^{n-1} prime(n-i) *
 *            (prime(i+1)-prime(i)) This function returns the n‑th term of
 *            OEIS sequence A067773 (offset 1)
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

/* cache for the first primes */
static uint64_t *primes = NULL;
static size_t   primes_cnt = 0;

/* Ensure that primes[0..need-1] are available.
   If not, enlarge the start limit (rough estimate 20*need). */
static void ensure_primes(size_t need)
{
    if (primes_cnt >= need)
        return;

    /* free old cache if necessary */
    if (primes) {
        free(primes);
        primes = NULL;
        primes_cnt = 0;
    }

    size_t limit = (need < 6) ? 15 : need * 20;
    build_primes(limit, &primes, &primes_cnt);

    /* increase the limit until we have enough primes */
    while (primes_cnt < need) {
        limit <<= 1;                        /* double the limit */
        free(primes);
        primes = NULL;
        build_primes(limit, &primes, &primes_cnt);
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset does not matter for n < 1 */
        return 0;

    /* build the required primes */
    ensure_primes((size_t)n);

    /* compute the formula */
    seq_t result = (seq_t)(2ULL * primes[n - 1]);   /* 2*prime(n) */

    for (idx_t i = 1; i < n; ++i) {
        seq_t p1   = (seq_t)primes[n - i - 1];
        seq_t diff = (seq_t)(primes[i] - primes[i - 1]); /* prime(i+1)-prime(i) */
        result += (seq_t)((uint64_t)p1 * diff);
    }

    return result;
}
