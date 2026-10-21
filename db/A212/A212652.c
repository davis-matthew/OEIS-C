/*
 * A212652  a(n) is the least positive integer M such that n = T(M) - T(k),
 *          for k an integer, 0 <= k <= M, where T(r) = A000217(r) is the r-th
 *          triangular number.
 * https://oeis.org/A212652
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{odd d|n} ( n/d + (d-1)/2 )
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the definition from OEIS A212652 by generating   */
/* all odd divisors of n and evaluating the expression for each.   */
/* The algorithm has O(τ(n)) time where τ(n) is the divisor count, */
/* and uses trial division to factor the odd part of n.           */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*  factor the odd part of n                                         */
    /* ------------------------------------------------------------------ */
    idx_t tmp = n;
    /* remove factor 2 */
    while ((tmp & 1) == 0) tmp >>= 1;

    idx_t primes[32];
    unsigned char exps[32];
    int pf_cnt = 0;

    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            unsigned char e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[pf_cnt] = p;
            exps[pf_cnt]   = e;
            ++pf_cnt;
        }
    }
    if (tmp > 1) {
        primes[pf_cnt] = tmp;
        exps[pf_cnt]   = 1;
        ++pf_cnt;
    }

    /* ------------------------------------------------------------------ */
    /*  generate all odd divisors of n                                    */
    /* ------------------------------------------------------------------ */
    size_t cap = 16;
    seq_t *divs = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!divs) return 0;          /* allocation failure, unlikely in tests */
    size_t dcnt = 1;
    divs[0] = 1;

    for (int i = 0; i < pf_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        unsigned char e = exps[i];

        size_t old = dcnt;
        seq_t mult = 1;
        for (unsigned char k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < old; ++j) {
                seq_t val = divs[j] * mult;
                if (dcnt + 1 > cap) {
                    cap <<= 1;
                    divs = (seq_t *)realloc(divs, cap * sizeof(seq_t));
                    if (!divs) return 0;   /* allocation failure */
                }
                divs[dcnt++] = val;
            }
        }
    }

    /* ------------------------------------------------------------------ */
    /*  evaluate the expression on each divisor and keep the minimum     */
    /* ------------------------------------------------------------------ */
    seq_t sn = (seq_t)n;
    seq_t best = 0;
    bool first = true;

    for (size_t i = 0; i < dcnt; ++i) {
        seq_t d = divs[i];
        seq_t val = sn / d + (d - 1) / 2;
        if (first || val < best) {
            best = val;
            first = false;
        }
    }

    free(divs);
    return best;
}
