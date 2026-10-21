/*
 * A201779  G.f. A(x) satisfies A(x/(1-x))=x*(1+A(x))/(1-A(x)-A(x)^2).
 * https://oeis.org/A201779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is defined implicitly by the functional equation A(x/(1-x))
 *            = x*(1+A(x))/(1-A(x)-A(x)^2), with offset 1 (a(1)=1). The
 *            implementation computes a(n) using an O(n^2) recurrence derived
 *            from series manipulation
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 21, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache of precomputed values.  a[0] is unused, a[1] is the first term. */
static seq_t *a_arr = NULL;   /* a[n] */
static seq_t *d_arr = NULL;   /* d[n] = –a[n] – Σ_{k=1}^{n-1} a[k]a[n-k]  */
static seq_t *c_arr = NULL;   /* c[n] coefficient of x^n in B/D where B=1+A, D=1‑A‑A² */
static size_t  CAP   = 0;      /* current capacity of the arrays          */
static idx_t   NEXT  = 1;      /* next index to compute; 1 denotes “none yet” */

/* Reallocate the cache to hold at least indices 0…n. */
static void ensure_cache(idx_t n)
{
    if (CAP > (size_t)n) return;
    size_t new_cap = CAP ? CAP * 2 : 64;
    while (new_cap <= (size_t)n) new_cap *= 2;
    a_arr = (seq_t *)realloc(a_arr, (new_cap + 1) * sizeof(seq_t));
    d_arr = (seq_t *)realloc(d_arr, (new_cap + 1) * sizeof(seq_t));
    c_arr = (seq_t *)realloc(c_arr, (new_cap + 1) * sizeof(seq_t));
    CAP   = new_cap;
}

/* Compute and return a(n).  The function is memoised and can be called
   in any order of n. */
seq_t a(idx_t n)
{
    if (n == 0)                     /* offset 1; a(0) is defined as 0 for consistency */
        return (seq_t)0;

    ensure_cache(n);

    /* Initialise the base case on the first call. */
    if (NEXT == 1) {
        a_arr[0] = (seq_t)0;         /* dummy, unused */
        d_arr[0] = (seq_t)1;         /* D(0)=1 */
        c_arr[0] = (seq_t)1;         /* C(0)=1 because B/D at 0 is 1 */
        a_arr[1] = (seq_t)1;         /* a(1)=1 */
        d_arr[1] = -(a_arr[1]);      /* d[1] = –a[1] */
        c_arr[1] = a_arr[1] - d_arr[1] * c_arr[0]; /* c[1] = a[1] – d[1]*c[0] */
        NEXT = 2;                    /* next index still to compute */
    }

    /* If requested n already computed, just return it. */
    if (n < NEXT) return a_arr[n];

    /* Compute values up to n, sequentially.  Each step uses the
       recurrence derived from the functional equation. */
    for (idx_t i = NEXT; i <= n; ++i) {
        /* 1. Compute a[i] using c[i-1] and the binomial-sum part. */
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;          /* binom(N-1,0) for N=i */
        for (idx_t k = 1; k < i; ++k) {
            sum += a_arr[k] * binom;
            /* update binomial coefficient: binom(N-1,k) = binom(N-1,k-1)*(N-1-(k-1))/k */
            binom = (binom * (seq_t)(i - k)) / (seq_t)k;
        }
        a_arr[i] = c_arr[i - 1] - sum;

        /* 2. Compute d[i] = –a[i] – Σ_{k=1}^{i-1} a[k]a[i-k]. */
        seq_t d_val = -a_arr[i];
        for (idx_t k = 1; k < i; ++k) {
            d_val -= a_arr[k] * a_arr[i - k];
        }
        d_arr[i] = d_val;

        /* 3. Compute c[i] via the series‑division recurrence:
           c[i] = b[i] – Σ_{k=1}^{i} d[k] * c[i-k], with b[i] = a[i] for i≥1. */
        seq_t c_val = a_arr[i];              /* b[i] = a[i] */
        for (idx_t k = 1; k <= i; ++k) {
            c_val -= d_arr[k] * c_arr[i - k];
        }
        c_arr[i] = c_val;
    }

    NEXT = n + 1;            /* all indices up to n are now computed */
    return a_arr[n];
}
