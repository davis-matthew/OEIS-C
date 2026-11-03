/*
 * A225903  The smallest number beginning with n whose distinct prime factors
 *          are the first n primes.
 * https://oeis.org/A225903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest number starting with decimal n whose distinct
 *            prime factors are exactly the first n primes
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 321 (2026-11-01)
 * Helpers:   build_primes_upto, num_digits, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm: 1) compute B = p1·p2·…·pn. 2) for an increasing
   number of trailing zeros k, search the interval
        [ n·10^k , (n+1)·10^k – 1 ]
   for the first multiple B·Y that lies in the interval and whose
   factorisation uses only primes ≤ pn.  */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
 
/* Utility headers provided by the framework */
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/num_digits.h"
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Check whether a 64‑bit integer y has no prime factor larger than
   the first n primes stored in primes[0 .. n‑1]. */
static bool is_smooth64(uint64_t y, size_t n, const uint64_t *primes)
{
    if (y <= 1) return true;          /* 1 is smooth */
    for (size_t i = 0; i < n; ++i) {
        uint64_t p = primes[i];
        if (p > y) break;             /* no need to test larger primes */
        while (y % p == 0)
            y /= p;
        if (y == 1) return true;      /* fully factored by first n primes */
    }
    return y == 1;                    /* remaining factor > p_n means not smooth */
}
 

 
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined, but return 0 */
 
    /* ---------------------------------------------------- */
    /* 1.  Ensure we have the first n primes                 */
    /* ---------------------------------------------------- */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0; /* number of primes stored in 'primes' */
 
    if ((size_t)n > prime_cnt) {
        idx_t lim = _prime_limit_for_idx(n);   /* upper bound for p_n   */
        uint64_t *tmp = NULL;
        size_t    tmpcnt = 0;
 
        build_primes_upto(lim, &tmp, &tmpcnt); /* primes <= lim      */
        if (primes) free(primes);
        primes = tmp;
        prime_cnt = tmpcnt;                   /* may be > n, but we only use first n */
    }
 
    /* ---------------------------------------------------- */
    /* 2.  Compute B = product of the first n primes       */
    /* ---------------------------------------------------- */
    seq_t B = (seq_t)1;
    for (size_t i = 0; i < (size_t)n; ++i)
        B = B * (seq_t)primes[i];
 
    /* ---------------------------------------------------- */
    /* 3.  Search for the smallest suitable multiple       */
    /* ---------------------------------------------------- */
    size_t digits_n = num_digits_idx(n);
    size_t digits_B = num_digits(B);
 
    /* Starting k is chosen so that 10^k is at least as large
       as the difference in digit lengths of B and n.  This
       often skips a few useless iterations.              */
    size_t k = (digits_B > digits_n) ? (digits_B - digits_n) : 0;
 
    while (1) {
        seq_t powk = pow10((idx_t)k);               /* 10^k            */
        seq_t L    = (seq_t)n * powk;                /* lower bound     */
        seq_t R    = ((seq_t)(n + 1)) * powk;       /* upper bound     */
 
        seq_t low  = (L + B - (seq_t)1) / B;        /* ceil(L/B)       */
        seq_t high = (R - (seq_t)1) / B;            /* floor((R-1)/B)  */
 
        /* Convert bounds to 64‑bit integers.  For the problem
           sizes they are guaranteed to fit.               */
        uint64_t low_u  = (uint64_t)low;
        uint64_t high_u = (uint64_t)high;
 
        if (low_u <= high_u) {
            for (uint64_t cand = low_u; cand <= high_u; ++cand) {
                if (is_smooth64(cand, (size_t)n, primes)) {
                    return B * (seq_t)cand;         /* Found the term */
                }
            }
        }
        /* No candidate in this interval – increase trailing zeros. */
        ++k;
    }
}
