/*
 * A032446  Number of solutions to phi(k) = 2n.
 * https://oeis.org/A032446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of solutions to φ(k) = 2n The implementation
 *            enumerates the prime‑power factors that can compose a totient
 *            value m=2n, recursively. Primes up to 2n are generated once with
 *            a sieve; the recursion respects non‑decreasing prime order to
 *            avoid duplicate counts
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* ---------------  static storage for the prime table  --------------- */
static uint64_t   *prime_arr  = NULL;   /* array of primes  */
static size_t     prime_cnt  = 0;       /* number of primes in prime_arr */
static idx_t      prime_limit = 0;     /* highest limit that has been pre‑computed */

/* Ensure that primes up to 'limit' (exclusive) are available */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit) return;
    /* free any previous allocation */
    if (prime_arr) free(prime_arr);
    build_primes_upto(limit, &prime_arr, &prime_cnt);
    prime_limit = limit;
}

/* ---------------  recursive counting --------------- */
static seq_t _rec(uint64_t rem, uint64_t min_p)
{
    if (rem == 1ULL) return 1;   /* nothing left to factor */

    seq_t total = 0;
    /* iterate over all primes >= min_p */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p < min_p) continue;
        /* Since (p-1) must divide rem, we may stop early */
        if (p - 1 > rem) break;

        uint64_t pow = 1ULL;       /* p^(e-1)  (e starts at 1) */
        for (;;) {
            /* factor = (p - 1) * p^(e-1) */
            uint64_t factor = (p - 1) * pow;
            if (factor > rem) break;
            if (rem % factor != 0) break;   /* no larger e will divide */

            seq_t sub = _rec(rem / factor, p + 1);
            total += sub;

            /* prepare for next exponent e+1 */
            if (pow > rem / p) break;       /* avoid overflow */
            pow *= p;
        }
    }
    return total;
}

/* ---------------  public entry point --------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* below offset – undefined by OEIS */

    idx_t m = 2 * n;                    /* target totient value */
    ensure_primes(m + 1ULL);            /* primes up to m+1 suffice */
    seq_t result = _rec((uint64_t)m, 2ULL);
    return result;
}
