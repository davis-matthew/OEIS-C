/*
 * A083748  Star numbers (A003154) whose digital sum and product of nonzero
 *          digits are both triangular numbers.
 * https://oeis.org/A083748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   int_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate star numbers s(k)=6k(k-1)+1, keeping those whose
   digit sum and product of non‑zero digits are both triangular numbers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/int_isqrt.h"

/* -------- simple digit utilities ------------------------------------------- */
static seq_t digit_sum(seq_t n)
/* sum of decimal digits of n */
{
    seq_t sum = 0;
    while (n > 0) {
        sum += n % 10;
        n /= 10;
    }
    return sum;
}

static seq_t prod_nonzero_digits(seq_t n)
/* product of the non‑zero decimal digits of n */
{
    unsigned long long prod = 1; /* use 64‑bit intermediate */
    while (n > 0) {
        unsigned int d = (unsigned int)(n % 10);
        if (d) prod *= d;
        n /= 10;
    }
    return (seq_t)prod;
}

/* -------- test for triangular number -------------------------------------- */
static bool is_triangular(seq_t v)
/* v is triangular iff 8*v + 1 is a perfect square */
{
    unsigned long long xv = (unsigned long long)v;
    unsigned long long t = xv * 8ULL + 1ULL;
    unsigned long long r = int_isqrt(t);
    return r * r == t;
}

/* -------- main sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *terms   = NULL;
    static size_t  cap    = 0;
    static size_t  count  = 0;
    static idx_t   next_k = 1;        /* next star‑number index to examine */

    /* generate terms until we have at least n of them */
    while (count < n) {
        /* ensure enough storage */
        if (count == cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;   /* safety fallback */
            terms = tmp;
            cap   = new_cap;
        }

        /* compute the k‑th star number: 6k(k–1)+1 */
        unsigned long long kk = (unsigned long long)next_k;
        unsigned long long star64 = 6ULL * kk * (kk - 1) + 1ULL;
        seq_t star = (seq_t)star64;

        /* test digit sum */
        if (is_triangular(digit_sum(star))) {
            /* test product of non‑zero digits */
            seq_t prod = prod_nonzero_digits(star);
            if (is_triangular(prod)) {
                terms[count++] = star;   /* store valid term */
            }
        }

        ++next_k;   /* advance to next k */
    }

    return terms[n - 1];   /* 1‑based indexing */
}
