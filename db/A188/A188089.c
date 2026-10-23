/*
 * A188089  Positions of 0 in the zero-one sequence [nr+4r]-[nr]-[4r], where
 *          r=sqrt(3), n>=1.
 * https://oeis.org/A188089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 4705, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The positions of 0 in the sequence defined by  
   s_n = ⌊n√3 + 4√3⌋ – ⌊n√3⌋ – ⌊4√3⌋ .  
   A zero occurs when {n√3} + {4√3} < 1, i.e. when {n√3} < 1 – {4√3}.  
   The function generates terms by scanning successive multiples of √3
   and recording those indices whose fractional part falls below the
   threshold.  This is an O(n) search that fills and enlarges a static
   cache of zero positions.  For each call it returns the n‑th zero.  */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* -------------------------------------------------------------- */
/*  Constants for √3 and the threshold 1 – frac(4√3)              */
/* -------------------------------------------------------------- */
static const long double SQRT3      = sqrtl(3.0L);          /* √3      */
static const long double FOUR_SQR3  = 4.0L * SQRT3;        /* 4√3     */
static const long double FP4        = FOUR_SQR3 -
                                      (long double)((unsigned long long)FOUR_SQR3); /* {4√3} */
static const long double THRESH     = 1.0L - FP4;          /* 1 – {4√3} */

/* -------------------------------------------------------------- */
/*  Static cache for zero positions                            */
/* -------------------------------------------------------------- */
static seq_t *zero_pos  = NULL;  /* array of stored zero positions          */
static size_t zero_cap  = 0;     /* allocated capacity of zero_pos          */
static size_t zero_len  = 0;     /* number of zeros currently stored        */
static idx_t   last_k   = 0;     /* last tested index (k)                  */

/* -------------------------------------------------------------- */
/*  Grow the cache to hold at least 'needed' entries             */
/* -------------------------------------------------------------- */
static void ensure_cap(size_t needed)
{
    if (needed <= zero_cap)
        return;
    size_t new_cap = zero_cap ? zero_cap * 2 : 16;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_buf = (seq_t *)realloc(zero_pos, new_cap * sizeof(seq_t));
    zero_pos = new_buf;
    zero_cap = new_cap;
}

/* -------------------------------------------------------------- */
/*  Return the n‑th zero position (1‑based index)                */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                 /* safety for out‑of‑range calls */
        return 0;

    /* Generate zeros until we have at least n entries */
    while (zero_len < (size_t)n) {
        ++last_k;                                   /* next index to test */

        long double frac = fmodl((long double)last_k * SQRT3, 1.0L);

        if (frac < THRESH) {                         /* zero occurs here */
            ensure_cap(zero_len + 1);
            zero_pos[zero_len++] = (seq_t)last_k;    /* store as seq_t */
        }
    }

    return zero_pos[n - 1];
}
