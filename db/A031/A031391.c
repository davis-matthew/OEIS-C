/*
 * A031391  a(n) = prime(10*n-3).
 * https://oeis.org/A031391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(10*n-3) (OEIS A031391)
 * Range:     u8 n <= 5, u16 n <= 654, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_arr = NULL;     /* primes up to prime_bound */
static size_t   prime_count = 0;       /* number of primes in prime_arr   */
static idx_t    prime_bound = 0;       /* numeric limit of the sieve       */
static idx_t    max_needed_idx = 0;    /* highest prime index held so far */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* safety; OEIS offset is 1 */

    /* target prime index = 10*n - 3 (counting from 1) */
    idx_t target = 10 * n - 3;
    if (target == 0) return 0;        /* unreachable for n>=1 */

    /* return directly if we already have enough primes */
    if (target <= max_needed_idx) {
        return (seq_t)prime_arr[target - 1];
    }

    /* need to grow the primes list to cover 'target' */
    idx_t required = target;

    /* estimate a numeric upper bound that surely contains required
       primes (Rosser's theorem: p_n < n(ln n + ln ln n) for n>=6) */
    idx_t bound;
    if (required < 6) {
        bound = 15;                    /* small fixed bound */
    } else {
        /* use a simple over‑estimate: 20*n is more than enough for n up to 10 000 */
        bound = required * 20;
    }

    /* rebuild the sieve if necessary */
    if (bound > prime_bound) {
        if (prime_arr) free(prime_arr);
        build_primes_upto(bound, &prime_arr, &prime_count);
        prime_bound = bound;
    }

    /* verify we have enough primes; if not, double the bound */
    while (prime_count < required) {
        bound *= 2;
        if (prime_arr) free(prime_arr);
        build_primes_upto(bound, &prime_arr, &prime_count);
        prime_bound = bound;
    }

    max_needed_idx = required;
    return (seq_t)prime_arr[target - 1];
}
