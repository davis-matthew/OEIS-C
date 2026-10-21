/*
 * A209914  Number of ways n can be written as a multiple of its reversal
 *          A004086(n) +/- a prime p < n.
 * https://oeis.org/A209914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of primes p ≤ n (including p = n when n is prime)
 *            such that r = rev(n) and either (n - p) is a positive multiple
 *            of r (k ≥ 1) or (n + p) is a positive multiple of r (k ≥ 1)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:  rev(n) is the decimal reversal of n (without leading zeros). */

#include <stdlib.h>
#include "../utils/rev.h"
#include "../utils/build_primes_upto.h"

static idx_t _prime_limit = 0;
static uint64_t *_prime_list = NULL;
static size_t _prime_count = 0;

/* Ensure all primes ≤ limit are available. */
static void ensure_primes(idx_t limit)
{
    if (limit <= _prime_limit)
        return;
    if (_prime_list)
        free(_prime_list);
    build_primes_upto(limit, &_prime_list, &_prime_count);
    _prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* a(0) and a(1) are 0 by definition.  */
    if (n <= 1)
        return (seq_t)0;

    idx_t r = rev(n);               /* reversed decimal digits of n */
    if (r == 0)                     /* n = 0 handled above, harmless here */
        return (seq_t)0;

    /* primes up to n are needed (p may equal n). */
    ensure_primes(n);

    seq_t count = 0;

    for (size_t i = 0; i < _prime_count; ++i) {
        idx_t p = _prime_list[i];
        if (p > n)        /* primes are sorted ascending */
            break;

        /* + case: n = k*r + p  with k ≥ 1 */
        if (n >= p) {                 /* ensures n-p non‑negative */
            idx_t diff = n - p;
            if (diff % r == 0) {
                idx_t k = diff / r;
                if (k >= 1)
                    ++count;
            }
        }

        /* - case: n = k*r - p  with k ≥ 1 */
        idx_t sum = n + p;
        if (sum % r == 0) {
            idx_t k = sum / r;
            if (k >= 1)
                ++count;
        }
    }

    return count;
}
