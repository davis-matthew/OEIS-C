/*
 * A219604  Smallest prime p such that 2n+1 = 4q + p for some odd prime q, or
 *          0 if no such prime exists.
 * https://oeis.org/A219604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest prime p such that 2n+1 = 4q + p for some
 *            prime q; returns 0 if no such prime exists
 * Range:     u8 n <= 162, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

static uint64_t *prime_list = NULL;   /* primes up to prime_limit */
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* 2n + 1, computed in seq_t to avoid overflow of idx_t */
    seq_t m = (seq_t)2 * (seq_t)n + 1;

    /* Build or extend the prime list to cover m (needed for both p and q) */
    if (m > (seq_t)prime_limit) {
        idx_t new_limit = (idx_t)m;
        if (prime_list != NULL) {
            free(prime_list);
            prime_list = NULL;
            prime_cnt  = 0;
        }
        build_primes_upto(new_limit, &prime_list, &prime_cnt);
        prime_limit = new_limit;
    }

    /* Search primes in increasing order for the smallest p that works */
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_list[i];          /* candidate p (prime) */
        if (p > m) break;                       /* no need to check larger p */

        seq_t rem = m - p;
        if (rem & 3) continue;                  /* 4 does not divide rem */

        seq_t q = rem >> 2;                     /* q = (m - p) / 4 */

        if (is_prime_big(q))
            return p;                           /* first qualifying p is minimal */
    }

    return (seq_t)0;                            /* no representation found */
}
