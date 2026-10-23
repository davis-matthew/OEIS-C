/*
 * A193527  G.f. A(x) satisfies 0 = f(A(x), A(x^2)) where f(u, v) = u * (u -
 *          2) - (v + 2).
 * https://oeis.org/A193527
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(2*n) = 0 (n>0), a(2*n-1) = b(n), b(1)=2, b(k) = ( a(k-1) -
 *            sum_{j=1}^{k-1} b(j)*b(k-j) ) / 2, where a(k-1)=b(k/2) if k is
 *            even, else 0
 * Range:     i8 n <= 12, i16 n <= 22, i32 n <= 42, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types idx_t and seq_t are provided by the build environment. */

/* -----------------------------------------------------------------------
   Array of values of the odd‑index subsequence b
   ----------------------------------------------------------------------- */
static idx_t       b_len   = 0;   /* highest index computed (0 unused) */
static idx_t       b_cap   = 0;   /* array capacity (highest available index) */
static seq_t *     b       = NULL;

/* Ensure the array b has capacity for indices up to `need` (inclusive). */
static void
ensure_b_capacity(idx_t need)
{
    if (need <= b_cap) return;
    size_t new_cap = (b_cap == 0) ? 8 : b_cap * 2;
    while (new_cap < need) new_cap *= 2;
    b = (seq_t *)realloc(b, sizeof(seq_t) * (new_cap + 1));
    b_cap = new_cap;
}

/* Compute values of b up to index m (inclusive). */
static void
compute_b_to(idx_t m)
{
    if (m <= b_len) return;          /* already done */

    if (b_len == 0) {                /* first initialization */
        ensure_b_capacity(2);
        b[0] = 0;                    /* unused dummy */
        b[1] = 2;                    /* b(1)=2 */
        b_len = 1;
    }

    for (idx_t k = b_len + 1; k <= m; ++k) {
        ensure_b_capacity(k);

        seq_t sum = 0;
        seq_t acomp;                  /* contribution of a(k-1) */

        if ((k & 1) == 0) {          /* k even -> k-1 odd -> a(k-1)=b(k/2) */
            acomp = b[k / 2];
        } else {                     /* k odd  -> k-1 even -> a(k-1)=0   */
            acomp = 0;
        }

        /* convolution sum: Σ_{j=1}^{k-1} b(j)*b(k-j) */
        for (idx_t j = 1; j < k; ++j) {
            sum += b[j] * b[k - j];
        }

        b[k] = (acomp - sum) / 2;
    }

    b_len = m;
}

/* -----------------------------------------------------------------------
   Public function required by the problem
   ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == -1) return (seq_t)1;    /* a(-1) = 1 */
    if (n ==  0) return (seq_t)1;    /* a(0)  = 1 */

    if (n > 0) {
        if ((n & 1) == 0) return (seq_t)0;   /* a(2*k) = 0 for k>0 */

        /* n odd: compute corresponding b(m), m=(n+1)/2 */
        idx_t m = (n + 1) >> 1;
        compute_b_to(m);
        return b[m];
    }

    /* values for n < -1 are not defined; return 0 */
    return (seq_t)0;
}
