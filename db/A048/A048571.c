/*
 * A048571  Triangle read by rows: T(n,k) = number of distinct prime factors
 *          of C(n,k).
 * https://oeis.org/A048571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of distinct prime factors of C(r,k), where (r,k)
 *            is the position of n in the triangle read by rows
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build a list of all primes ≤ limit.  The helpers are defined in
 * ../utils/build_primes_upto.h
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static uint64_t *prime_cache = NULL;     /* primes up to current limit   */
static size_t      prime_cnt   = 0;       /* number of primes in cache   */
static idx_t       prime_limit = 0;       /* current cache limit         */

/* Ensure prime_cache holds all primes ≤ limit, reallocating if needed. */
static void
ensure_prime_cache(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    free(prime_cache);
    prime_cache = NULL;
    prime_cnt   = 0;
    prime_limit = limit;

    build_primes_upto(limit, &prime_cache, &prime_cnt);
}

/* Return a(n).  The function works for any idx_t n, in any call order. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Index of the first element in row r. */
    idx_t row_start = r * (r + 1) / 2;
    idx_t k         = n - row_start;    /* column in the row */

    /* C(r,0) = C(r,r) = 1 has zero distinct prime factors. */
    if (k == 0 || k == r)
        return (seq_t)0;

    /* Prime list up to r is needed for prime factorisation. */
    ensure_prime_cache(r);

    /* Count distinct primes dividing C(r,k).
     * For each prime p, exponent in C(r,k) is v_p(r!) - v_p(k!) - v_p((r-k)!).
     */
    int count = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > r)
            break;

        uint64_t exp = 0;
        uint64_t t;

        t = r;
        while (t) {
            t /= p;
            exp += t;
        }
        t = k;
        while (t) {
            t /= p;
            exp -= t;
        }
        t = r - k;
        while (t) {
            t /= p;
            exp -= t;
        }

        if (exp > 0)
            ++count;
    }

    return (seq_t)count;
}
