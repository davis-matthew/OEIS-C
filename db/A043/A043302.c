/*
 * A043302  Triangular table of 2^n *(n+k)! / ((n-k)! * k! * 4^k).
 * https://oeis.org/A043302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 34, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   fact_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043302.  Triangular table of 2^n *(n+k)! / ((n-k)! * k! * 4^k)
   enumerated row by row.  The function converts the 1‑based index n
   into row r and column k and returns the corresponding table entry. */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/fact_u.h"

/* Helper: compute 2^exp using shifts.  Works for any non‑negative
   exponent; uses only integer operations. */
static seq_t
pow2_ui(idx_t exp) {
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r <<= 1;
    return r;
}

/* Return the a(n) term for the OEIS sequence A043302.
   n is 1‑based; for n < 1, behaviour is undefined. */
seq_t
a(idx_t n) {
    /* Find row r such that (r+1)(r+2)/2 >= n > r(r+1)/2.  For small n
       a floating‑point sqrt is safe and fast. */
    idx_t r = (idx_t)((sqrt((double)(8.0 * (double)n - 7.0)) - 1.0) / 2.0);

    /* Compute k, the column within the row. */
    idx_t prev = r * (r + 1) / 2;           /* total terms before this row */
    idx_t k = n - prev - 1;                 /* 0‑based column */

    /* Evaluate the formula:
          T(r, k) = 2^r * (r+k)! / ((r-k)! * k! * 4^k)          */
    seq_t num = fact_u(r + k);                /* (r + k)!          */
    seq_t den1 = fact_u(r - k);               /* (r - k)!          */
    seq_t den2 = fact_u(k);                   /* k!                */

    seq_t res = num / den1;                   /* (r+k)!/(r-k)!     */
    res = res / den2;                         /* /k!                */

    /* Multiply by 2^r and divide by 4^k (which equals 2^(2k)). */
    seq_t mult = pow2_ui(r);                  /* 2^r               */
    seq_t div = pow2_ui(2 * k);               /* 4^k = 2^(2k)      */

    res = res * mult;                         /* *2^r              */
    res = res / div;                          /* ÷4^k              */

    return res;
}
