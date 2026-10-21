/*
 * A076353  Numbers n such that Sum_{k=1..n} d(k) is an integer, where d(k) is
 *          the decimal fraction 0.floor(sqrt(k)) (e.g. d(16)=0.4).
 * https://oeis.org/A076353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer k for which sum_{i=1..k}
 *            floor(sqrt(i)) / 10^{digits(floor(sqrt(i)))} is an integer
 * Range:     u8 n <= 19, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   digits_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sum is integer iff
   sum_{i=1..k} floor(sqrt(i)) * 10^{max_digits - digits(floor(sqrt(i)))} is divisible
   by 10^{max_digits},  where max_digits is the maximum number of decimal digits
   of floor(sqrt(i)) seen so far.  we maintain this sum modulo the current base
   10^{max_digits}. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_of.h"

static const seq_t POW10[21] = {
    1ULL, 10ULL, 100ULL, 1000ULL, 10000ULL, 100000ULL, 1000000ULL,
    10000000ULL, 100000000ULL, 1000000000ULL, 10000000000ULL,
    100000000000ULL, 1000000000000ULL, 10000000000000ULL,
    100000000000000ULL, 1000000000000000ULL, 10000000000000000ULL,
    100000000000000000ULL, 1000000000000000000ULL, 10000000000000000000ULL,
    100000000000000000000ULL
};

seq_t a(idx_t n)
{
    /* static state for memoisation --------------------------- */
    static seq_t *terms = NULL;   /* stored terms */
    static size_t sz = 0;         /* number of terms stored       */
    static size_t cap = 0;        /* allocated capacity           */

    /* progress through integer k --------------------------------- */
    static idx_t last_k = 0;          /* last k processed          */
    static idx_t curr_sqrt = 0;       /* floor(sqrt(last_k))       */
    static idx_t next_sq = 1;         /* (curr_sqrt+1)^2           */

    static int digits_max = 1;      /* maximum digits of curr_sqrt seen so far */
    static seq_t base = 10;         /* 10^digits_max */
    static seq_t cum_mod = 0;       /* cumulative sum modulo base */

    /* grow the terms array if required -------------------------- */
    if (sz + 1 > cap) {
        size_t new_cap = (cap == 0) ? 64 : cap << 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        terms = tmp;
        cap = new_cap;
    }

    /* generate consecutive terms until we have at least n of them -- */
    while (sz < (size_t)n) {
        idx_t k = last_k + 1;

        /* advance curr_sqrt when k passes a new perfect square */
        while (k >= next_sq) {
            ++curr_sqrt;
            /* carefully avoid overflow of squares */
            next_sq = (curr_sqrt + 1) * (curr_sqrt + 1);
        }

        /* determine digits of current floor sqrt */
        int digits = digits_of(curr_sqrt);

        /* if a longer digit length now appears, increase base and rescale */
        if (digits > digits_max) {
            int delta = digits - digits_max;
            seq_t factor_increase = POW10[delta];
            /* grow cumulative sum accordingly */
            cum_mod = (cum_mod * factor_increase) % POW10[digits];
            digits_max = digits;
            base = POW10[digits_max];
        }

        /* factor needed for the current term: 10^{digits_max - digits} */
        seq_t factor = POW10[digits_max - digits];
        seq_t contrib = (seq_t)curr_sqrt * factor;

        cum_mod = (cum_mod + contrib) % base;

        if (cum_mod == 0) {
            terms[sz++] = (seq_t)k;
        }

        last_k = k;
    }

    return terms[n - 1];
}
