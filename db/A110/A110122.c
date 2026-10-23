/*
 * A110122  Number of Delannoy paths of length n with no EE's crossing the
 *          line y = x (i.e., no two consecutive E steps from the line y = x+1
 *          to the line y = x-1).
 * https://oeis.org/A110122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (2*n^2+9*n+7)*a(n) - (26*n^2+135*n+151)*a(n+1) * +
 *            (88*n^2+528*n+746)*a(n+2) - (26*n^2+177*n+277)*a(n+3) * +
 *            (2*n^2+15*n+25)*a(n+4) = 0 * Initial values: a(0)=1, a(1)=3,
 *            a(2)=12, a(3)=53
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;          /* array holding computed terms */
static idx_t  cap = 0;           /* allocated capacity          */
static idx_t  len = 0;           /* number of terms already computed (a[0..len-1]) */

/* Ensure that the array has at least 'need' elements allocated. */
static void ensure_capacity(idx_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = (size_t)(cap ? cap * 2 : 8);
    while ((idx_t)new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* allocation failed – terminate */
    A = tmp;
    cap = (idx_t)new_cap;
}

/* Main entry point required by the build system */
seq_t a(idx_t n)
{
    /* Initialise the first four terms on first call */
    if (len == 0) {
        ensure_capacity(4);
        A[0] = (seq_t)1;
        A[1] = (seq_t)3;
        A[2] = (seq_t)12;
        A[3] = (seq_t)53;
        len = 4;
    }

    if (n < len)
        return A[n];

    /* Extend the array up to n */
    ensure_capacity(n + 1);
    for (idx_t k = len; k <= n; ++k) {
        /* We compute a[k] from a[k-1]..a[k-4] using the recurrence with
         * t = k-4 to keep polynomial arguments non‑negative. */
        uint64_t t = (uint64_t)(k - 4);   /* t = n-4 in the recurrence */
        seq_t c0 = (seq_t)( 2ULL * t * t + 15ULL * t + 25ULL); /* denom */
        seq_t c1 = (seq_t)(26ULL * t * t + 177ULL * t + 277ULL);
        seq_t c2 = (seq_t)(88ULL * t * t + 528ULL * t + 746ULL);
        seq_t c3 = (seq_t)(26ULL * t * t + 135ULL * t + 151ULL);
        seq_t c4 = (seq_t)( 2ULL * t * t +  9ULL * t + 7ULL);

        seq_t term1 = c1 * A[k - 1];
        seq_t term2 = c2 * A[k - 2];
        seq_t term3 = c3 * A[k - 3];
        seq_t term4 = c4 * A[k - 4];

        /* Numerator = c1*a[k-1] - c2*a[k-2] + c3*a[k-3] - c4*a[k-4] */
        seq_t num = term1 - term2 + term3 - term4;

        A[k] = num / c0;                 /* exact division by recurrence */
    }
    len = n + 1;
    return A[n];
}
