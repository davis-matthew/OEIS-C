/*
 * A225163  Denominators of the sequence s(n) of the sum resp. product of
 *          fractions f(n) defined recursively by f(1) = 3/1; f(n+1) is chosen
 *          so that the sum and the product of the first n terms of the
 *          sequence are equal.
 * https://oeis.org/A225163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of the partial sum/product s(n) defined
 *            recursively by f(1)=3/1 and f(n+1) chosen so that sum_{i=1}^n
 *            f(i) = prod_{i=1}^n f(i). Let b(1)=1/3 and b(n)=b(n-1)-b(n-1)^2;
 *            then a(n) is the numerator of the fraction b(n) in lowest terms
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* External linkage function required by OEIS */
seq_t a(idx_t n);

/* Static storage for memoisation of numerator (a(n)) and denominator
   of b(n).  Index 0 is unused. */
static seq_t *p_arr = NULL;   /* numerators a(n) */
static seq_t *q_arr = NULL;   /* denominators of b(n) */
static idx_t  computed_max_n = 0;

/* Ensure the arrays are allocated and large enough for indices
   up to `need` (inclusive).  The arrays are 1‑based. */
static void ensure_capacity(idx_t need)
{
    if (need <= computed_max_n)
        return;

    /* Allocate new arrays.  Cast the result of malloc for C++ compatibility. */
    seq_t *new_p = (seq_t*)malloc((need + 1) * sizeof(seq_t));
    seq_t *new_q = (seq_t*)malloc((need + 1) * sizeof(seq_t));
    if (!new_p || !new_q)
        return;  /* allocation failed – caller will see uninitialized data */

    /* Copy over previously computed values. */
    for (idx_t i = 1; i <= computed_max_n; ++i) {
        new_p[i] = p_arr[i];
        new_q[i] = q_arr[i];
    }

    /* Free old storage (free(NULL) is safe). */
    free(p_arr);
    free(q_arr);

    p_arr = new_p;
    q_arr = new_q;
}

/* Return the nth term of OEIS sequence A225163. */
seq_t a(idx_t n)
{
    if (n < 1)            /* out of bounds – return 0 as a safety net */
        return (seq_t)0;

    /* Ensure storage for index `n`. */
    ensure_capacity(n);

    /* Initialise the sequence if this is the first call. */
    if (computed_max_n == 0) {
        p_arr[1] = (seq_t)1; /* numerator of b(1)=1/3 */
        q_arr[1] = (seq_t)3; /* denominator of b(1)=1/3 */
        computed_max_n = 1;
    }

    /* Compute terms up to n lazily. */
    for (idx_t i = computed_max_n + 1; i <= n; ++i) {
        seq_t p_prev = p_arr[i - 1];
        seq_t q_prev = q_arr[i - 1];

        /* b(i) = p_prev/q_prev - (p_prev/q_prev)^2
           --> numerator = p_prev*(q_prev - p_prev)
           --> denominator = q_prev^2 */
        seq_t diff  = q_prev - p_prev;
        seq_t numer = p_prev * diff;    /* numerator of next b(i) */
        seq_t denom = q_prev * q_prev;  /* denominator of next b(i) */

        /* Reduce the fraction by gcd. */
        seq_t g = gcd(numer, denom);

        p_arr[i] = numer / g;  /* numerator = a(i) */
        q_arr[i] = denom / g;  /* denominator of b(i), kept for future steps */
    }

    computed_max_n = (n > computed_max_n) ? n : computed_max_n;

    /* a(n) is the numerator of b(n). */
    return p_arr[n];
}
