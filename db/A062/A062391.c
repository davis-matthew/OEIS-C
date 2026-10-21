/*
 * A062391  a(1) = 3, a(2) = 5; a(n+1) = smallest prime number > a(n) such
 *          that the sum of any three consecutive terms is a prime.
 * https://oeis.org/A062391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=5; a(n+1)= smallest prime > a(n) such that *
 *            a(n-1)+a(n)+a(n+1) is prime. The first two terms are initialized
 *            * and each subsequent term is found by testing successive odd
 *            primes
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Needed helpers provided by the test harness. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/*  Cache the sequence; 10000 slots are sufficient for n up to 10000. */
static seq_t  cache[10000];
static size_t cache_len = 0;

/*  Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* unused in OEIS */

    /* Initialize the two base terms if this is the first call. */
    if (cache_len == 0) {
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)5;
        cache_len = 2;
    }

    /* Generate terms until we reach the requested index. */
    while (cache_len < n) {
        seq_t   prev      = cache[cache_len - 1];
        seq_t   prevprev  = cache[cache_len - 2];

        /* Check that the underlying integer fits in idx_t. */
        idx_t   pv   = (idx_t)prev;
        idx_t   ppv  = (idx_t)prevprev;

        idx_t cand = pv + 1;                 /* first candidate > prev */
        if (cand <= 2) cand = 3;
        if ((cand & 1ULL) == 0) cand++;      /* skip even numbers */

        /* Find the next prime satisfying the triple‑sum condition. */
        while (true) {
            if (is_prime(cand) && is_prime(ppv + pv + cand)) {
                cache[cache_len++] = (seq_t)cand;
                break;
            }
            cand += 2;                       /* test next odd integer */
        }
    }

    return cache[n - 1];
}
