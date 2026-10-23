/*
 * A038741  T(n,n-4), array T as in A038738.
 * https://oeis.org/A038741
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-4} b_k * C(n-4-k+7,7) * where the coefficients
 *            b_k satisfy b_0 = 1, b_1 = 3, * b_k = 3*b_{k-1} - b_{k-2} for k
 *            ≥ 2. * The values a(n) are computed iteratively using this *
 *            recurrence, which for j = 0..7 defines * A_j(m) = Σ_{k=0}^m b_k
 *            * C(m-k+7, j) * and satisfies * A_j(m) = A_...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External integer types supplied by the build system */
extern seq_t _dummy;   /* silence unused variable warnings */

/* Pre‑computed binomial coefficients C(7,j) for j = 0..7 */
static const seq_t C7[8] = { (seq_t)1, (seq_t)7, (seq_t)21, (seq_t)35,
                             (seq_t)35, (seq_t)21, (seq_t)7, (seq_t)1 };

/* Cached values of the b_k series                     */
static seq_t *b_cache = NULL;   /* b_cache[i] = b_i                */
static idx_t  b_max   = 0;      /* largest index computed so far   */

/* Cached sequence a(n) values                          */
static seq_t *a_cache = NULL;   /* a_cache[n] = a(n)                */
static idx_t  a_max   = 0;      /* largest n computed so far       */

/* Current state of the A_j(m) array (m = current maximal n-4) */
static seq_t A[8];
static idx_t m_cur   = 0;       /* current m = a_max - 4            */

/* ----------------------------------------------------------
 * Ensure that b_cache contains coefficients up to index m
 * ---------------------------------------------------------- */
static void ensure_b_cache(idx_t m)
{
    if (b_cache == NULL) {           /* first allocation */
        b_cache = (seq_t *)malloc((m + 1) * sizeof(seq_t));
        b_cache[0] = (seq_t)1;       /* b0 = 1 */
        if (m >= 1)
            b_cache[1] = (seq_t)3;   /* b1 = 3 */
        b_max = (m >= 1) ? 1 : 0;
    } else if (b_max < m) {          /* grow and fill the rest */
        b_cache = (seq_t *)realloc(b_cache, (m + 1) * sizeof(seq_t));
        for (idx_t k = b_max + 1; k <= m; ++k) {
            if (k == 1) {
                b_cache[k] = (seq_t)3;
            } else {
                seq_t three_prev = b_cache[k - 1] + b_cache[k - 1] + b_cache[k - 1];
                b_cache[k] = three_prev - b_cache[k - 2];
            }
        }
        b_max = m;
    }
}

/* ----------------------------------------------------------
 * Ensure that a_cache can store values up to index n
 * ---------------------------------------------------------- */
static void ensure_a_cache(idx_t n)
{
    if (a_cache == NULL) {                  /* first allocation */
        a_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= n; ++i)
            a_cache[i] = (seq_t)0;
    } else if (a_max < n) {                 /* grow and zero new part */
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = a_max + 1; i <= n; ++i)
            a_cache[i] = (seq_t)0;
    }
}

/* ----------------------------------------------------------
 * Initialize all state the first time a() is called.
 * ---------------------------------------------------------- */
static void init_state(idx_t n)
{
    idx_t m_target = (n >= 4) ? (n - 4) : 0;   /* the largest m needed */
    ensure_b_cache(m_target);
    ensure_a_cache(n);

    /* set A_j(0) = C(7,j) for j=0..7   */
    for (int j = 0; j <= 7; ++j)
        A[j] = C7[j];

    a_cache[4] = A[7];      /* a(4) = 1 */
    a_max   = 4;
    m_cur   = 0;
}

/* ----------------------------------------------------------
 * Main public routine: a(n)
 * ---------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 4)          /* behaviour below offset is irrelevant   */
        return (seq_t)0;

    /* First call: initialise */
    if (a_cache == NULL) {
        init_state(n);
        return a_cache[n];
    }

    if (n <= a_max)     /* already computed */
        return a_cache[n];

    /* Need to compute values from a_max+1 up to n */
    idx_t target_m = n - 4;
    ensure_b_cache(target_m);
    ensure_a_cache(n);

    for (idx_t m = m_cur + 1; m <= target_m; ++m) {
        seq_t bval = b_cache[m];

        /* Update A_j from j=7 down to 1 so that
           A_{j-1} remains unchanged when it is used.        */
        for (int j = 7; j >= 1; --j) {
            A[j] = A[j] + A[j - 1] + bval * C7[j];
        }
        A[0] = A[0] + bval;        /* A_0(m) = A_0(m-1) + b_m            */

        a_cache[m + 4] = A[7];     /* a(n) = A_7(m)                       */
    }

    a_max = n;
    m_cur = target_m;

    return a_cache[n];
}
