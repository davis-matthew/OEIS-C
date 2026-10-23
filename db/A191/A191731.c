/*
 * A191731  Dispersion of A047204, (numbers >1 and congruent to 3 or 4 mod 5),
 *          by antidiagonals.
 * https://oeis.org/A191731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 78, u32 n <= 300, u64 n <= 1225,
 *            u128 n <= 4753, big any n
 * Helpers:   apply_f, ceil_triangular_idx, get_T
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A191731: Dispersion of A047204 (numbers >1 and ≡ 3 or 4 mod 5),
   read along antidiagonals.  For a given index n (1‑based) let
   (i,j) be the antidiagonal coordinates:
        n = T(i+j‑2) + i          (T(m)=m(m+1)/2)
        i = n – T(i+j‑3)
        j = (i+j‑2) – i + 1
   The corresponding value is the (j‑1)-fold iterate of A047204 applied
   to the i‑th element of the complement sequence T'={1,2,5,6,7,10,…},
   i.e. numbers not congruent to 3 or 4 mod 5.  The implementation
   uses only integer arithmetic (seq_t can be arbitrary‑precision). */

#include <stddef.h>  /* for size_t, though not needed here */
#include "../utils/apply_f.h"
#include "../utils/ceil_triangular_idx.h"
#include "../utils/get_T.h"

/* Forward declarations of helper routines */
static inline seq_t apply_f(seq_t k);
static inline seq_t get_T(idx_t i);
static inline idx_t ceil_triangular_idx(idx_t n);

/* Main routine: a(n) returns the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* Determine antidiagonal d (i+j‑1) such that T(d-1) < n ≤ T(d) */
    idx_t d = ceil_triangular_idx(n);

    /* Triangular number before this diagonal */
    unsigned __int128 tri_before = (unsigned __int128)(d - 1) * d / 2;
    idx_t i = n - (idx_t)tri_before;          /* row index (i)   */
    idx_t j = d - i + 1;                      /* column index (j) */

    /* Starting value from the complement sequence T' */
    seq_t val = get_T(i);

    /* Iterate A047204 (j‑1) times */
    for (idx_t step = 1; step < j; ++step)
        val = apply_f(val);

    return val;
}

/* ----------------------------------------------------------------- */
/*  Helper: computes T(i), the i‑th element of the complement sequence */

/* ----------------------------------------------------------------- */

/* ----------------------------------------------------------------- */
