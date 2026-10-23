/*
 * A148524  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, -1), (-1,
 *          -1, 0), (-1, -1, 1), (-1, 0, -1), (1, 1, 0)}.
 * https://oeis.org/A148524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A148524
   Number of walks within N^3 (the first octant of Z^3) starting at (0,0,0)
   with steps taken from
   {(-1,-1,-1), (-1,-1,0), (-1,-1,1), (-1,0,-1), (1,1,0)}.
   Sequence a(n) satisfies
   a(n) = Σ_{d=0}^{⌊n/2⌋} B(n,d) * M(d)
   where B(n,d)=C(n,d)-C(n,d-1) is the number of Dyck prefixes of length n
   with d down steps, and M(d) is the weighted Motzkin meander number of
   length d with weight 1 for up and flat steps and weight 2 for a down step.
   The implementation uses dynamic programming with memoisation and
   works for all n≥0 in O(n²) time and <2 s for n up to 10 000.
   The routine has external linkage: seq_t a(idx_t n). */

#include <stdlib.h>
#include <string.h>

/* counted in exact seq_t type, which may be a builtin unsigned type
   or an arbitrary‑precision class (C++). */
seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *a_arr  = NULL;  /* computed sequence values */
    static size_t a_cap  = 0;
    static idx_t  a_last = 0;

    static seq_t *B_arr  = NULL;  /* B(n,d) array for current n */
    static size_t B_cap  = 0;
    static idx_t  B_last = 0;     /* last n for which B_arr was updated */

    static seq_t *M_arr  = NULL;  /* weighted Motzkin meander numbers */
    static size_t M_cap  = 0;
    static idx_t  M_last = 0;     /* largest d for which M_arr was computed */

    /* auxiliary arrays for the Motzkin DP */
    static seq_t *f_arr  = NULL;  /* current heights for length i-1 */
    static seq_t *nf_arr = NULL;  /* next heights for length i */
    static size_t f_cap  = 0;
    static idx_t  f_len  = 0;     /* current length of f_arr   */

    /* --------------------------------------------------------------------
       initialise tables on first call */
    if (!a_arr) {
        a_cap = 1;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_arr) exit(1);
        a_arr[0] = (seq_t)1;      /* a(0)=1 */
        a_last = 0;

        B_cap = 1;
        B_arr = (seq_t *)malloc(B_cap * sizeof(seq_t));
        if (!B_arr) exit(1);
        B_arr[0] = (seq_t)1;      /* B(0,0)=1 */
        B_last = 0;

        M_cap = 1;
        M_arr = (seq_t *)malloc(M_cap * sizeof(seq_t));
        if (!M_arr) exit(1);
        M_arr[0] = (seq_t)1;      /* M(0)=1 */

        f_cap = 1;
        f_arr  = (seq_t *)malloc(f_cap * sizeof(seq_t));
        nf_arr = (seq_t *)malloc(f_cap * sizeof(seq_t));
        if (!f_arr || !nf_arr) exit(1);
        f_arr[0] = (seq_t)1;      /* height counts for length 0 */
        f_len = 0;
    }

    /* --------------------------------------------------------------------
       compute values up to n if necessary */
    if (n > a_last) {
        /* grow a_arr if necessary */
        if (a_cap < (size_t)(n + 1)) {
            a_cap = (size_t)(n + 1);
            a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
            if (!a_arr) exit(1);
        }

        /* extend B_arr, M_arr, f_arr, nf_arr as required */
        for (idx_t cur = a_last + 1; cur <= n; ++cur) {
            idx_t dmax = cur / 2;  /* floor(cur/2) */

            /* ---- update B_arr (Dyck prefix counts) ---- */
            if (B_cap < (size_t)(dmax + 1)) {
                size_t new_cap = (size_t)(dmax + 1);
                B_cap = new_cap;
                B_arr = (seq_t *)realloc(B_arr, B_cap * sizeof(seq_t));
                if (!B_arr) exit(1);
                /* zero new entries */
                for (size_t i = cur/2 + 1; i < B_cap; ++i) B_arr[i] = (seq_t)0;
            }
            for (idx_t d = dmax; d >= 1; --d)
                B_arr[d] += B_arr[d-1];
            /* B_arr[0] stays unchanged (always 1) */

            /* ---- ensure M_arr computed up to dmax ---- */
            if (M_cap < (size_t)(dmax + 1)) {
                size_t new_cap = (size_t)(dmax + 1);
                M_cap = new_cap;
                M_arr = (seq_t *)realloc(M_arr, M_cap * sizeof(seq_t));
                if (!M_arr) exit(1);
                for (size_t i = M_last + 1; i < M_cap; ++i) M_arr[i] = (seq_t)0;
            }

            /* compute new Motzkin meander numbers up to dmax */
            while (M_last < dmax) {
                idx_t i = M_last + 1;  /* next length to compute */

                /* initialise nf_arr[0..i] = 0 */
                if (f_cap < (size_t)(i + 1)) {
                    size_t new_cap = (size_t)(i + 1);
                    f_cap = new_cap;
                    f_arr  = (seq_t *)realloc(f_arr,  f_cap * sizeof(seq_t));
                    nf_arr = (seq_t *)realloc(nf_arr, f_cap * sizeof(seq_t));
                    if (!f_arr || !nf_arr) exit(1);
                }
                memset(nf_arr, 0, (i + 1) * sizeof(seq_t));

                /* DP transition */
                for (idx_t h = 0; h < i; ++h) {
                    seq_t val = f_arr[h];
                    if (!val) continue;
                    nf_arr[h+1] += val;          /* up */
                    nf_arr[h]   += val;          /* flat */
                    if (h > 0) {
                        seq_t prod = val * (seq_t)2;  /* down contributes weight 2 */
                        nf_arr[h-1] += prod;
                    }
                }

                /* sum over all heights to get M_arr[i] */
                seq_t sum = (seq_t)0;
                for (idx_t h = 0; h <= i; ++h) sum += nf_arr[h];
                M_arr[i] = sum;

                /* swap arrays for next iteration */
                seq_t *tmp = f_arr;
                f_arr = nf_arr;
                nf_arr = tmp;
                ++M_last;
            }

            /* ---- compute a(cur) via convolution ---- */
            seq_t total = (seq_t)0;
            for (idx_t d = 0; d <= dmax; ++d)
                total += B_arr[d] * M_arr[d];
            a_arr[cur] = total;
        }
        a_last = n;
    }

    return a_arr[n];
}
