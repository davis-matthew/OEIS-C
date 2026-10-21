/*
 * A056902  Numbers n where 36n^2+24n+7 is prime (sorted by absolute values
 *          with negatives before positives).
 * https://oeis.org/A056902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 28, i32 n <= 28, i64 n <= 60, i128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056902: numbers m such that 36*m^2+24*m+7 is prime.
   The terms are ordered by increasing absolute value, with negative
   terms listed before their positive counterparts.  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Return the n-th member of the sequence (n ≥ 0). */
seq_t a(idx_t n)
{
    /* Cache of already-computed terms. */
    static seq_t *terms = NULL;
    static size_t len   = 0;   /* number of terms stored */
    static size_t cap   = 0;   /* allocated capacity */
    static idx_t  next_k = 0;  /* next absolute value to test */
    static int    next_sign = 0; /* 0 → negative, 1 → positive */
    static bool   init = false;

    /* Initialisation: 0 is always a term. */
    if (!init) {
        cap = 16;
        terms = (seq_t *)malloc(cap * sizeof(seq_t));
        terms[0] = (seq_t)0;
        len    = 1;
        next_k = 1;
        next_sign = 0;   /* start with the negative of 1 */
        init = true;
    }

    /* Generate terms until we have at least n+1 of them. */
    while (len <= n) {
        seq_t m;
        /* Pick next candidate: negative then positive for a given |m|. */
        if (next_sign == 0) {          /* negative side */
            m = -(seq_t)next_k;
            next_sign = 1;
        } else {                       /* positive side */
            m = (seq_t)next_k;
            next_sign = 0;
            next_k++;                  /* advance to next absolute value */
        }

        /* Compute test value: 36*m^2 + 24*m + 7.  Use a wider type if
           possible to avoid overflow of the underlying seq_t. */
        seq_t expr;
#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
        /* If sizeof(seq_t) fits within 64‑bit, use 128‑bit arithmetic. */
        if (sizeof(seq_t) <= 8) {
            /* m is finite in this case; cast to a signed 64‑bit int. */
            __int128 mm = (__int128)((long long)m);
            __int128 v128 = 36 * mm * mm + 24 * mm + 7;
            expr = (seq_t)v128;               /* implicit conversion to seq_t */
        } else
#endif
        {
            /* For arbitrary‑precision seq_t, rely on its own operators. */
            expr = ((seq_t)36 * m * m) + ((seq_t)24 * m) + (seq_t)7;
        }

        /* If the value is prime, store the corresponding m. */
        if (is_prime_big(expr)) {
            if (len == cap) {
                cap *= 2;
                terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
            }
            terms[len++] = m;
        }
    }

    return terms[n];
}
