/*
 * A117269  Triangle T, read by rows, that satisfies matrix equation: T -
 *          (T-I)^2 = C, where C is Pascal's triangle.
 * https://oeis.org/A117269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (0‑based) n-th entry of the triangle T read by rows,
 *            where T(n,k) = A052886(n‑k) * C(n,k) for n>k, C(n,k) =
 *            binomial(n,k). C(n,n)=1. Hence a(n) = T(r,k) with 0≤k≤r, r
 *            chosen so that r(r+1)/2 ≤ n < (r+1)(r+2)/2 and k = n – r(r+1)/2.
 *            T(r,k) = left_edge[r‑k] * binomial(r,k), where left_e...
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Check that the type seq_t provides the usual arithmetic operators
   and the EOF.  It is supplied by the build system.                */

static seq_t *left_edge = NULL;       /* stored values T(m,0)  */
static idx_t  left_len  = 0;          /* number of entries computed   */
static idx_t  left_cap  = 0;          /* allocated capacity           */

/* Compute binomial(n,k) for a built‑in integer type seq_t.
   The algorithm uses the multiplicative formula and exact integer division. */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n - k)
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    return res;
}

/* Ensure that left_edge[0..max] are computed. */
static void ensure_left(idx_t max)
{
    if (left_edge == NULL) {
        /* initial allocation */
        left_cap = (max < 4 ? 8 : (idx_t)1 << 3);   /* at least 8 elements */
        left_edge = (seq_t*)malloc(sizeof(seq_t) * left_cap);
        if (!left_edge) abort();
        left_edge[0] = (seq_t)1;   /* T(0,0)  */
        left_edge[1] = (seq_t)1;   /* T(1,0)  */
        left_len   = 2;
    }
    /* enlarge storage if necessary */
    if (left_len <= max) {
        if (left_cap <= max) {
            while (left_cap <= max)
                left_cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(left_edge, sizeof(seq_t) * left_cap);
            if (!tmp) abort();
            left_edge = tmp;
        }
        /* compute remaining entries */
        for (idx_t n = left_len; n <= max; ++n) {
            if (n == 0) {
                left_edge[0] = (seq_t)1;
                continue;
            }
            if (n == 1) {
                left_edge[1] = (seq_t)1;
                continue;
            }
            seq_t sum   = (seq_t)1;   /* the “+1” term */
            seq_t binom = (seq_t)1;   /* binomial(n,1) initially */
            for (idx_t j = 1; j < n; ++j) {
                /* update binom from binom(n,j-1) to binom(n,j) */
                binom = (binom * (seq_t)(n - j + 1)) / (seq_t)j;
                /* term = left_edge[j] * left_edge[n-j] * binom */
                seq_t term = left_edge[j];
                term = term * left_edge[n - j];
                term = term * binom;
                sum += term;
            }
            left_edge[n] = sum;
        }
        left_len = max + 1;
    }
}

seq_t a(idx_t n)
{
    /* find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n)
        ++r;
    /* base = r(r+1)/2, k = n - base */
    idx_t base = ((unsigned __int128)r * (r + 1)) / 2;
    idx_t k = n - base;

    /* we need left_edge values up to r */
    ensure_left(r);

    /* compute binomial(r,k) */
    seq_t bin = binom_small(r, k);

    /* result = left_edge[r-k] * binomial(r,k) */
    seq_t res = left_edge[r - k] * bin;
    return res;
}
