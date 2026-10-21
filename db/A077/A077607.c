/*
 * A077607  Convolutory inverse of the factorial sequence.
 * https://oeis.org/A077607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077607 – convolutory inverse of the factorial sequence
 *  Definition:  a(1)=1,
 *              a(n) = -∑_{k=1}^{n-1} (n-k+1)! · a(k)   for n≥2
 *
 *  The function keeps static arrays for the values a(k) and for the
 *  factorials k!; the arrays are grown with grow_seq_array().  All
 *  arithmetic is done in type seq_t, which may be an arbitrary‑precision
 *  integer class or a native signed integer type.  The code is valid in
 *  both C11 and C++17.
 *
 *  For big integer builds, the helper grow_seq_array() allocates a new
 *  array and default‑constructs the elements; for native builds it
 *  simply reallocates and copies values.  No floating point or external
 *  libraries are used.
 */

#include <stdlib.h>                /* size_t, NULL */
#include "../utils/grow_seq_array.h"      /* void grow_seq_array(seq_t **, size_t *, size_t); */

seq_t a(idx_t n)
{
    static seq_t *A     = NULL;   /* a values, 1‑based, A[0] unused */
    static size_t Acap  = 0;      /* allocated size of A         */
    static idx_t  Aused = 0;      /* highest index computed      */

    static seq_t *fact  = NULL;   /* factorials, 1‑based (fact[1]=1) */
    static size_t Factcap = 0;    /* allocated size of fact      */
    static idx_t  Factused = 0;   /* highest factorial computed  */

    if (n < 1) return 0;          /* the sequence is undefined for n<1 */

    /* ---------- grow the factorial array up to n ---------- */
    if ((idx_t)Factcap < n + 1) {
        grow_seq_array((seq_t **)&fact, &Factcap, (size_t)n + 1);
    }

    if (Factused == 0) {
        fact[1] = (seq_t)1;       /* 1! = 1 */
        Factused = 1;
    }
    /* compute missing factorials */
    for (idx_t i = Factused + 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;   /* i! = (i-1)! * i */
    }
    Factused = n;

    /* ---------- grow the a‑array up to n ---------- */
    if ((idx_t)Acap < n + 1) {
        grow_seq_array((seq_t **)&A, &Acap, (size_t)n + 1);
    }

    if (Aused == 0) {
        A[1] = (seq_t)1;          /* a(1) = 1 */
        Aused = 1;
    }

    /* ---------- compute a(m) for m = Aused+1 … n ---------- */
    for (idx_t m = Aused + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < m; ++k) {
            idx_t idx = m - k + 1;    /* factorial index */
            sum += fact[idx] * A[k];
        }
        A[m] = -sum;
    }
    Aused = n;

    return A[n];
}
