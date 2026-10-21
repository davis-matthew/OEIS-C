/*
 * A089595  Table T(n,k), n>=0 and k>=0: Stern's diatomic array read by
 *          antidiagonals (version 5).
 * https://oeis.org/A089595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   floor_tri_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A089595 – Stern’s diatomic array read by antidiagonals (version 5)
 *
 *  The array T(n,k) satisfies
 *
 *      T(0,0)=1,   T(0,1)=0,
 *      T(n,2k)  = T(n‑1,k) = T(n,k)−S(k),
 *      T(n,2k+1)=T(n,2k)+T(n,2k+2),
 *
 *  where S(k) is Stern’s diatomic sequence (A002487).  
 *  For all n,k≥0 one has
 *
 *      T(n,k) = T0(k) + n * S(k),
 *  
 *  with T0(k) = T(0,k) computed from the above recurrence.
 *
 *  The OEIS sequence a(n) is obtained by reading the table along
 *  antidiagonals; for n≥0 let
 *
 *      d = ⌊(√(8n+1)−1)/2⌋  (largest d with d(d+1)/2 ≤ n)
 *      i = n − d(d+1)/2     (row index inside diagonal d)
 *      j = d − i            (column index inside diagonal d)
 *
 *  then
 *
 *      a(n) = T(i,j) = T0(j) + i * S(j).
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_tri_root.h"

/* ---------- helper: integer triangular root -------------------------------- */

/* ---------- Stern’s diatomic sequence ------------------------------------- */

static seq_t *stern      = NULL;
static size_t stern_cap  = 0;   /* allocated capacity */
static size_t stern_len  = 0;   /* number of elements computed (last index +1) */

static inline void ensure_stern_upto(idx_t upto)
{
    if ((size_t)upto >= stern_len) {
        size_t needed = (size_t)upto + 1;
        if (needed > stern_cap) {
            size_t new_cap = stern_cap ? stern_cap : 1;
            while (new_cap < needed) new_cap <<= 1;
            stern = (seq_t *)realloc(stern, new_cap * sizeof(seq_t));
            stern_cap = new_cap;
        }
        for (size_t i = stern_len; i <= (size_t)upto; ++i) {
            if (i == 0) stern[i] = 0;
            else if (i == 1) stern[i] = 1;
            else if ((i & 1) == 0)      /* even */
                stern[i] = stern[i >> 1];
            else                        /* odd  */
                stern[i] = stern[i >> 1] + stern[(i >> 1) + 1];
        }
        stern_len = (size_t)upto + 1;
    }
}

/* ---------- first row T0(k) ---------------------------------------------- */

static seq_t *t0          = NULL;
static size_t t0_cap      = 0;   /* allocated capacity */
static size_t t0_len      = 0;   /* number of elements computed */

static inline void ensure_t0_upto(idx_t upto)
{
    if ((size_t)upto >= t0_len) {
        size_t needed = (size_t)upto + 1;
        if (needed > t0_cap) {
            size_t new_cap = t0_cap ? t0_cap : 1;
            while (new_cap < needed) new_cap <<= 1;
            t0 = (seq_t *)realloc(t0, new_cap * sizeof(seq_t));
            t0_cap = new_cap;
        }
        for (size_t i = t0_len; i <= (size_t)upto; ++i) {
            if (i == 0)                t0[i] = 1;
            else if (i == 1)           t0[i] = 0;
            else if ((i & 1) == 0) {   /* even  */
                size_t k = i >> 1;
                t0[i] = t0[k] - stern[k];
            } else {                   /* odd   */
                size_t k = (i - 1) >> 1;
                t0[i] = (t0[k] - stern[k]) + (t0[k + 1] - stern[k + 1]);
            }
        }
        t0_len = (size_t)upto + 1;
    }
}

/* ---------- public routine ------------------------------------------------- */

seq_t a(idx_t n)
{
    /* Diagonal index */
    idx_t d   = floor_tri_root(n);
    idx_t tri = d * (d + 1) / 2;          /* elements before this diagonal */
    idx_t i   = n - tri;                  /* row inside diagonal d */
    idx_t j   = d - i;                    /* column inside diagonal d */

    /* Build tables up to required indices */
    ensure_stern_upto((size_t)j);
    ensure_t0_upto((size_t)j);

    /* Compute a(n) = T(i,j) */
    return t0[j] + (seq_t)i * stern[j];
}
