/*
 * A191542  Dispersion of (2*floor(n*sqrt(3))), by antidiagonals.
 * https://oeis.org/A191542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 46, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   floor_sqrt, mul_small, rows_needed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates OEIS sequence A191542.
   It builds the sequence s(k) = 2*floor(k*sqrt(3)), its (infinite) complement t,
   constructs the dispersion matrix D whose n-th row is (t(n), s(t(n)), s(s(t(n))), …),
   and then outputs the n-th element in the antidiagonal enumeration of D.
   The function works for any idx_t/seq_t pair and does not rely on pre‑computed tables. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/mul_small.h"   /* seq_t mul_small(seq_t x, unsigned int m); */
#include "../utils/floor_sqrt.h" /* seq_t floor_sqrt(seq_t x); */
#include "../utils/rows_needed.h"

/* Computes s(x) = 2*floor(x*sqrt(3)) for any seq_t x.
   Uses only integer arithmetic. */
static inline seq_t s_func(seq_t x)
{
    /* Compute 3*x*x */
    seq_t sq  = x * x;          /* x squared (seq_t multiplication) */
    sq        = mul_small(sq, 3U);  /* multiply by 3 */

    /* floor(sqrt(3*x*x)) */
    seq_t f   = floor_sqrt(sq);

    /* 2 * floor(...)  */
    return mul_small(f, 2U);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* a(1) is the first term; n should be >=1. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Number of rows required to contain n antidiagonal elements. */
    idx_t m = rows_needed(n);

    /* Allocate temporary arrays for t and current row values. */
    seq_t *t   = (seq_t*)malloc((m + 1) * sizeof(seq_t));
    seq_t *cur = (seq_t*)malloc((m + 1) * sizeof(seq_t));
    if (!t || !cur) {
        free(t);
        free(cur);
        return (seq_t)0;
    }

    /* ----------------------------------------------------------------
       Build the complement sequence t(1), t(2), …, t(m).
       We generate s(k) on the fly and compare with a running integer
       candidate to decide whether the candidate belongs to t.         */
    idx_t k_s     = 1;                 /* index into s */
    seq_t s_val   = s_func((seq_t)k_s);
    idx_t cand    = 1;                 /* current integer to test */
    idx_t t_cnt   = 0;                 /* how many t values collected */

    while (t_cnt < m) {
        seq_t cand_val = (seq_t)cand;

        /* Advance s(k) until it reaches or surpasses cand */
        while (s_val < cand_val) {
            ++k_s;
            s_val = s_func((seq_t)k_s);
        }

        /* If s(k) > cand, then cand is not in s → it belongs to t */
        if (s_val != cand_val) {
            t[++t_cnt] = cand_val;
        }
        ++cand;
    }

    /* ----------------------------------------------------------------
       Initialize current values for each row to the corresponding t(i). */
    for (idx_t i = 1; i <= m; ++i) {
        cur[i] = t[i];
    }

    /* ----------------------------------------------------------------
       Enumerate the antidiagonals until we reach the n-th element. */
    idx_t count = 0;
    seq_t result = (seq_t)0;

    for (idx_t sum = 1; ; ++sum) {
        idx_t i_max = (sum <= m) ? sum : m;
        for (idx_t i = 1; i <= i_max; ++i) {
            ++count;
            if (count == n) {
                result = cur[i];
                goto cleanup;
            }
            /* Advance to the next element in this row. */
            cur[i] = s_func(cur[i]);
        }
    }

cleanup:
    free(t);
    free(cur);
    return result;
}
