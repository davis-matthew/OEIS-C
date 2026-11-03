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
 * Validated: standard, pass, 74 terms through n = 4596 (2026-11-01)
 * Helpers:   none
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

/*  Helper: applies the A047204 mapping once.
    A047204[k] is the k‑th integer >1 that ≡ 3 or 4 (mod 5).       */
static inline seq_t apply_f(seq_t k)
{
    seq_t block = (k - (seq_t)1) / (seq_t)2;    /* (k‑1)/2           */
    seq_t pos   = (k - (seq_t)1) % (seq_t)2;    /* (k‑1) mod 2        */
    return block * (seq_t)5 + (seq_t)3 + pos;   /* 5*block + 3 + pos */
}

/*  Helper: minimal integer d with d(d+1)/2 ≥ n                         */
static inline idx_t ceil_triangular_idx(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/*  T' = {1,2,5,6,7,10,11,12,15,16,17,20,…}                        */
static inline seq_t get_T(idx_t i)
{
    if (i == 1) return (seq_t)1;
    if (i == 2) return (seq_t)2;

    /* For i ≥ 3: pattern 5*c + pos where pos = (i-3) mod 3,
       c = (i-3)/3 + 1                                                   */
    idx_t pos = (i - 3) % 3;
    idx_t c   = (i - 3) / 3 + 1;

    /* Compute value using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 val128 = (unsigned __int128)5 * c + pos;
    return (seq_t)val128;
}

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
