/*
 * A131634  n-th even semiprime minus n-th odd semiprime.
 * https://oeis.org/A131634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A100484(n) - A046315(n) (even semiprime minus odd
 *            semiprime)
 * Range:     i8 none, i16 none, i32 none, i64 n <= 1000, i128 n <= 1000,
 *            big any n
 * Helpers:   build_primes_upto, compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compare_seq_t.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes        = NULL;
static size_t   primes_len     = 0;
static seq_t  *odd_semiprimes = NULL;
static size_t   odd_len       = 0;
static int      initialized   = 0;

static void init_sequence(void)
{
    /* 1 000 000 is enough to obtain at least 10 000 odd semiprimes
       and the 10 000th prime.  */
    const uint64_t LIMIT = 1000000;

    /* Generate all primes up to LIMIT.  */
    build_primes_upto(LIMIT, &primes, &primes_len);

    /* Build all odd semiprimes <= LIMIT.  */
    size_t cap = 1024;
    odd_semiprimes = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!odd_semiprimes) exit(1);
    size_t len = 0;

    for (size_t i = 1; i < primes_len; ++i) {      /* primes[i] is odd */
        uint64_t p = primes[i];
        for (size_t j = i; j < primes_len; ++j) {
            uint64_t q = primes[j];
            seq_t prod = (seq_t)p * (seq_t)q;
            if (prod > (seq_t)LIMIT) break;
            if (len == cap) {
                cap <<= 1;
                odd_semiprimes = (seq_t *)realloc(odd_semiprimes, cap * sizeof(seq_t));
                if (!odd_semiprimes) exit(1);
            }
            odd_semiprimes[len++] = prod;
        }
    }
    odd_len = len;

    /* Sort the odd semiprimes.  */
    qsort(odd_semiprimes, odd_len, sizeof(seq_t), compare_seq_t);

    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) init_sequence();
    if (n == 0) return 0;                 /* safety guard, OEIS offset is 1 */

    /* Ensure the requested index is available.  */
    if ((idx_t)primes_len < n || (idx_t)odd_len < n) return 0;

    /* Even semiprime: 2 * n-th prime. */
    seq_t even_sp = (seq_t)2 * (seq_t)primes[n - 1];

    /* Odd semiprime: n-th element of the sorted list. */
    seq_t odd_sp = odd_semiprimes[n - 1];

    return even_sp - odd_sp;
}
