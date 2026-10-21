/*
 * A095958  Twin prime pairs concatenated in decimal representation.
 * https://oeis.org/A095958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A095958:
   a(n) = concatenation of the nth twin prime pair (p, p+2).  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

/* Static storage for the prime table and twin–prime start values. */
static idx_t *primes = NULL;        /* list of primes */
static size_t prime_cnt = 0;        /* number of primes stored */
static idx_t  prime_limit = 0;      /* current bound used for the prime list */

static idx_t *twin_first = NULL;    /* list of the first prime in each twin pair */
static size_t twin_cnt  = 0;        /* number of twin pairs stored */

/* Rebuild the prime list up to 'new_limit' and rebuild the twin list. */
static void recompute(idx_t new_limit)
{
    /* free old data */
    if (primes) {
        free(primes);
        primes = NULL;
    }
    if (twin_first) {
        free(twin_first);
        twin_first = NULL;
    }

    /* build the new prime list */
    build_primes(new_limit, &primes, &prime_cnt);

    /* allocate space for at most 'prime_cnt' twin starts */
    twin_first = (idx_t *)malloc(prime_cnt * sizeof(idx_t));
    size_t t = 0;
    for (size_t i = 0; i + 1 < prime_cnt; ++i) {
        if (primes[i] + 2 == primes[i + 1])
            twin_first[t++] = primes[i];
    }
    twin_cnt   = t;
    prime_limit = new_limit;
}

/* Ensure that at least 'need' twin pairs have been found. */
static void ensure_twins(idx_t need)
{
    if (twin_cnt >= need)
        return;                       /* already enough */

    idx_t limit = prime_limit ? prime_limit * 2 : 100;
    while (twin_cnt < need) {
        recompute(limit);
        if (twin_cnt >= need)
            break;
        limit *= 2;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset 1; undefined for n = 0, return 0 for safety */
        return 0;

    ensure_twins(n);

    idx_t p  = twin_first[n - 1];   /* nth twin prime first element */
    idx_t p2 = p + 2;

    /* number of decimal digits of (p+2) */
    int dlen = digit_len((seq_t)p2);
    /* 10^dlen */
    seq_t pow = pow10((idx_t)dlen);

    /* concatenate p and p2: p * 10^dlen + p2 */
    seq_t result = (seq_t)p * pow + (seq_t)p2;
    return result;
}
