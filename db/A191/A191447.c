/*
 * A191447  Dispersion of (1+[n*sqrt(5)]), where [ ]=floor, by antidiagonals.
 * https://oeis.org/A191447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: uses Beatty sequence s(n)=1+⌊n*√5⌋ and its complement t(n).
   The dispersion matrix D has rows i with entries
      D(i,1)=t(i), D(i,2)=s(t(i)), …
   A(n) is the n-th entry read by rising antidiagonals of D.
   This implementation generates required t values on demand and
   evaluates s(v) via integer arithmetic (using floor_sqrt). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Compute s(x) = 1 + floor(x*√5) using the identity
   floor(x*√5) = floor(√(5*x²)) for x ≥ 0. */
static inline seq_t smile_of(seq_t x)
{
    seq_t prod = x * x;
    prod = prod * (seq_t)5;
    return floor_sqrt(prod) + (seq_t)1;
}

/* Storage of the complement sequence t(i): 1‑based via offset 0. */
static seq_t *t_arr      = NULL;    /* t(i) values */
static idx_t  t_size     = 0;       /* number of values stored */
static size_t t_capacity = 0;       /* allocated capacity in elements */

static idx_t t_gen_next  = 1;          /* next natural number to examine */
static idx_t t_gen_s_idx = 1;          /* next index in the Beatty sequence */
static seq_t t_gen_s_val = 0;          /* current value s(t_gen_s_idx) */

/* Ensure t_arr contains at least 'needed' elements (t(1)…t(needed)). */
static void t_ensure_up_to(idx_t needed)
{
    if (t_size >= needed)
        return;

    /* Grow the array if necessary */
    if (t_capacity < needed) {
        size_t new_cap = needed > 16 ? needed : 16;
        seq_t *new_arr = (seq_t *)realloc(t_arr, new_cap * sizeof(seq_t));
        t_arr = new_arr;
        t_capacity = new_cap;
    }

    /* Initialise s value on the first call */
    if (t_gen_s_val == 0) {
        t_gen_s_val = smile_of((seq_t)t_gen_s_idx);
    }

    /* Generate missing t(i) values */
    for ( ; t_size < needed ; ++t_gen_next ) {
        if ((seq_t)t_gen_next == t_gen_s_val) {
            /* this number belongs to s, skip it */
            ++t_gen_s_idx;
            t_gen_s_val = smile_of((seq_t)t_gen_s_idx);
        } else {
            t_arr[t_size++] = (seq_t)t_gen_next;
        }
    }
}

/* Main function: return the n-th term of A191447 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* below offset: undefined – return 0 */

    /* Find the antidiagonal r containing the n-th element.
       Number of elements up to diagonal r is r*(r+1)/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    const idx_t prev = (r - 1) * r / 2;      /* elements before diagonal r */
    const idx_t pos  = n - prev;             /* 1‑based position on diagonal r */
    idx_t i = pos;                           /* row index (1‑based)          */
    idx_t j = r + 1 - pos;                   /* column index (1‑based)       */

    t_ensure_up_to(i);                       /* make sure t(i) exists        */
    seq_t val = t_arr[i - 1];                /* D(i,1)                        */

    for (idx_t step = 1; step < j; ++step) {   /* apply s (j-1) times          */
        val = smile_of(val);
    }

    return val;
}
