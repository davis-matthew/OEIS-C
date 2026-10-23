/*
 * A170109  Number of reduced words of length n in Coxeter group on 4
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A170109 – number of reduced words of length n in the
 *  Coxeter group on 4 generators with relations S_i^2 = (S_i S_j)^38 = I.
 *
 *  Generating function:
 *      G(t) = (t^38 + 2 t^37 + … + 2 t + 1) / (3 t^38 – 2 t^37 – … – 2 t + 1)
 *
 *  From D(t)·G(t)=N(t) we obtain the recurrence for n ≥ 0:
 *
 *      a(n) = 2 · ∑_{k=1}^{37} a(n‑k)   –   3 · a(n‑38)   +   N_n
 *
 *  where N_n is the coefficient of t^n in N(t):
 *
 *          N_n = 1  if n = 0 or n = 38,
 *          N_n = 2  if 1 ≤ n ≤ 37,
 *          N_n = 0  otherwise.
 *
 *  The implementation evaluates the recurrence iteratively using a
 *  sliding window of the last 37 terms.  It works for any integer type
 *  defined as `seq_t` (8–128 bit or arbitrary precision) and is
 *  compatible with both C11 and C++17.  Allocation of the temporary
 *  array uses `new` when compiled as C++ and `malloc` otherwise,
 *  guaranteeing correct construction and destruction of class types.
 */

#include <stddef.h>   /* for size_t    */
#include <stdlib.h>   /* for malloc, free */

/* --- Allocation helpers -------------------------------------------------- */
#ifdef __cplusplus
/* In C++ use the compiler‑provided allocation so that constructors are
 * called and destructors are executed on delete. */
static inline seq_t *seq_alloc(__SIZE_TYPE__ count) {
    return new seq_t[count];
}
static inline void seq_free(seq_t *p) {
    delete [] p;
}
#else
/* In plain C use malloc/free (seq_t is always trivially constructible). */
static inline seq_t *seq_alloc(__SIZE_TYPE__ count) {
    return (seq_t*)malloc(count * sizeof(seq_t));
}
static inline void seq_free(seq_t *p) {
    free(p);
}
#endif

/* ------------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the trivial case.  All other cases use the recurrence. */
    if (n == 0) return (seq_t)1;

    /* Allocate space for a[0 .. n].  The array is big enough because
     * the recurrence needs a[i‑38] only when i ≥ 38, which is certainly
     * ≤ n, and all indices are within the allocated range. */
    seq_t *a = seq_alloc(n + 1);
    if (!a) return (seq_t)0;         /* allocation failure – fallback   */

    a[0] = (seq_t)1;                 /* a(0) = 1 */

    /* Sliding‑window sum of the last 37 terms.  For i = k the window
     * contains a(k‑1), a(k‑2), …, a(k‑37).  For k ≤ 37 the window
     * simply accumulates earlier terms. */
    seq_t window = (seq_t)0;

    for (idx_t i = 1; i <= n; ++i) {
        /* Add the newest previous term to the window. */
        window += a[i - 1];

        /* Keep only the most recent 37 terms.  When i > 37 the term
         * a[i‑38] leaves the window. */
        if (i > 37) {
            window -= a[i - 38];
        }

        /* Determine the contribution from the numerator N(t). */
        seq_t tval = (seq_t)0;
        if (i == 38)           tval = (seq_t)1;
        else if (i <= 37)      tval = (seq_t)2;

        /* Apply the recurrence.  a[i] depends on the current window
         * (sum over the previous 37 terms), on a[i‑38] (if i ≥ 38),
         * and on tval. */
        seq_t val = tval + (seq_t)2 * window;
        if (i >= 38) {
            val -= (seq_t)3 * a[i - 38];
        }
        a[i] = val;
    }

    seq_t result = a[n];
    seq_free(a);
    return result;
}
