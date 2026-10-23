/*
 * A177927  3-Monica numbers.
 * https://oeis.org/A177927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  3-Monica numbers
 *  A177927: composite positive integers k for which 3 divides S(k)-Sp(k)
 *  where S(k) is the sum of the decimal digits of k and
 *  Sp(k) is the sum of the decimal digits of each prime factor of k
 *  (counted with multiplicity).  The sequence is enumerated in increasing
 *  order of k.  No formula is known; we generate the numbers in
 *  ascending order by brute‑force search.
 *
 *  The function uses a small cache to avoid recomputation when called
 *  with arbitrary indices (in any order).  It works for 64‑bit integer
 *  values and also for arbitrary‑precision builds because all values
 *  are stored in the cache as `seq_t`, and the helper function
 *  `digit_sum_uint64` works on 64‑bit integers which are sufficient
 *  for the first many thousand terms.
 *
 *  The algorithm:
 *      •  Start testing from 4.
 *      •  For each candidate `k`, if it is composite and
 *         3 divides S(k)-Sp(k), append it to the cache.
 *      •  Stop when the cache contains `n` terms.
 *      •  Return the (n‑1)-th element of the cache.
 *
 *  No global state is modified except for the static cache and the
 *  current testing position.
 */

/* Allow ourselves to use the usual stdlib functions. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Helper functions ----- */

/* Simple primality test for 64‑bit integers. */
static bool is_prime_small(uint64_t n)
{
    if (n < 2)                 return false;
    if (n == 2 || n == 3)      return true;
    if ((n & 1) == 0)          return false;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0)        return false;
    return true;
}

/* Sum of decimal digits of a 64‑bit integer. */
static uint64_t digit_sum_uint64(uint64_t n)
{
    uint64_t sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

/* Test whether 3 divides S(k)-Sp(k).  k must be a composite integer. */
static bool cond_ok(uint64_t k)
{
    uint64_t Sk = digit_sum_uint64(k);

    /* Factorize k by trial division. */
    uint64_t temp = k;
    uint64_t Sp  = 0;
    uint64_t d   = 2;
    while (d * d <= temp) {
        if (temp % d == 0) {
            uint64_t exp = 0;
            uint64_t s   = digit_sum_uint64(d);
            while (temp % d == 0) {
                temp /= d;
                exp++;          /* count multiplicity */
            }
            Sp += s * exp;
        }
        d = (d == 2) ? 3 : d + 2;   /* skip even numbers after 2 */
    }
    if (temp > 1) {
        /* Remaining prime factor. */
        Sp += digit_sum_uint64(temp);
    }

    int64_t diff = (int64_t)Sk - (int64_t)Sp;
    int mod = diff % 3;
    if (mod < 0) mod += 3;
    return mod == 0;
}

/* ----- Main sequence function ----- */

seq_t a(idx_t n)
{
    /* Static cache of found 3‑Monica numbers. */
    static seq_t *cache = NULL;
    static size_t len   = 0;     /* length of cache */
    static size_t cap   = 0;     /* capacity of cache */
    static uint64_t cur = 4;     /* next candidate to test */

    /* Lazy initialization of cache. */
    if (cache == NULL) {
        cap   = 128;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Grow cache until we have at least `n` terms. */
    while (len < n) {
        /* Find the next composite number satisfying the condition. */
        for (;;) {
            if (!is_prime_small(cur)) {
                if (cond_ok(cur)) {
                    if (len == cap) {
                        cap *= 2;
                        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                    }
                    cache[len++] = (seq_t)cur;
                }
            }
            ++cur;
            break;   /* exit inner loop after advancing cur */
        }
    }

    return cache[n - 1];
}
