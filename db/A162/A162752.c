/*
 * A162752  a(1)=2^2. a(n) = the smallest p^q, p and q primes, that is >
 *          a(n-1), and where the base (p) of a(n) is the exponent (q) of
 *          a(n-1).
 * https://oeis.org/A162752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A162752: a(1)=2^2=4.
   a(n) = the smallest p^q, where p and q are primes,
   p is the exponent of a(n‑1) and q is the smallest prime such that p^q > a(n‑1). */

#include <math.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime.h"

/* Return a(n) for n≥1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n<1, return 0 */
    if (n == 1) return (seq_t)4;          /* 2^2 */

    /* current values for the previous term */
    uint64_t prev_base   = 2;             /* base of a(n-1)   */
    uint64_t prev_exp    = 2;             /* exponent of a(n-1) */
    long double log_prev = (long double)prev_exp * logl((long double)prev_base);

    /* iterate up to n, computing only logarithms */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t base = prev_exp; /* base of a(i) is exponent of a(i-1) */
        long double log_base = logl((long double)base);

        /* Find the smallest prime q such that base^q > a(i-1) */
        long double thr = log_prev / log_base;
        uint64_t cand = (uint64_t)floorl(thr) + 1;
        if (cand < 2) cand = 2;
        while (!is_prime((idx_t)cand)) ++cand;

        uint64_t curr_exp = cand;          /* exponent of a(i) */

        /* Update for next iteration */
        prev_base  = base;
        prev_exp   = curr_exp;
        log_prev   = (long double)curr_exp * log_base;
    }

    /* After the loop, prev_base holds exponent of a(n-1) and prev_exp the exponent of a(n) */
    /* a(n) = prev_base ^ prev_exp */
    return big_pow((seq_t)prev_base, (seq_t)prev_exp);
}
