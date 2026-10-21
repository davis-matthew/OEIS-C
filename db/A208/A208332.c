/*
 * A208332  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208333; see the Formula section.
 * https://oeis.org/A208332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle recurrence for T(n,k) T(n,k) = T(n-1,k) + 2*T(n-1,k-1)
 *            - 2*T(n-2,k-1) + 2*T(n-2,k-2) with T(1,0)=1, T(1,1)=0, T(2,0)=1,
 *            T(2,1)=1, T(2,2)=0. The sequence a(n) is the list of the nonzero
 *            T(n,k) in row‑by‑row order, starting with n=1. The first few
 *            terms are: 1,1,1,1,1,4,
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *precomp = NULL;       /* array holding sequence values */
static idx_t  precomp_cap = 0;      /* allocated capacity */
static idx_t  precomp_len = 0;      /* number of values stored */

/* previous two rows of the triangle (row indices are 0‑based) */
static seq_t *prev1 = NULL;       /* row n-1 */
static seq_t *prev2 = NULL;       /* row n-2 */
static idx_t  prev1_len = 0;      /* length of prev1 (n-1+1) */
static idx_t  prev2_len = 0;      /* length of prev2 (n-2+1) */
static idx_t  next_row = 1;       /* next row to compute (starts with 1) */

/* Ensure that precomp has space for at least 'need' more elements. */
static void grow_precomp(idx_t need)
{
    if (precomp_cap >= precomp_len + need)
        return;
    idx_t new_cap = precomp_cap ? precomp_cap : 8;
    while (new_cap < precomp_len + need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(precomp, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* memory exhausted */
    precomp = tmp;
    precomp_cap = new_cap;
}

/* Incrementally build rows until we have at least 'desired' terms. */
static void ensure_to(idx_t desired)
{
    while (precomp_len < desired) {
        idx_t r = next_row;       /* current row index (>=1) */
        idx_t len = r + 1;        /* full length of T(r,k) for k=0..r */
        seq_t *curr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!curr) abort();

        if (r == 1) {                     /* base row 1 */
            curr[0] = (seq_t)1;
            curr[1] = (seq_t)0;
        } else if (r == 2) {              /* base row 2 */
            curr[0] = (seq_t)1;
            curr[1] = (seq_t)1;
            curr[2] = (seq_t)0;
        } else {                          /* recurrence for r>=3 */
            for (idx_t k = 0; k < len; ++k) {
                seq_t a = (k < prev1_len)         ? prev1[k] : (seq_t)0;
                seq_t b = (k > 0 && (k-1) < prev1_len) ? prev1[k-1] : (seq_t)0;
                seq_t c = (k > 0 && (k-1) < prev2_len) ? prev2[k-1] : (seq_t)0;
                seq_t d = (k > 1 && (k-2) < prev2_len) ? prev2[k-2] : (seq_t)0;
                curr[k] = a + (seq_t)2 * b - (seq_t)2 * c + (seq_t)2 * d;
            }
        }

        /* Number of non-zero entries: always len-1 (the last one is zero) */
        idx_t to_append = len - 1;
        grow_precomp(to_append);
        for (idx_t i = 0; i < to_append; ++i)
            precomp[precomp_len++] = curr[i];

        /* Shift row buffers for next iteration */
        seq_t *old_prev2 = prev2;
        prev2 = prev1;
        prev2_len = prev1_len;
        prev1 = curr;
        prev1_len = len;

        free(old_prev2);          /* free row that is no longer needed */
        ++next_row;
    }
}

/*   a(n)   returns the n‑th term of the sequence (1‑based index)   */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    if (n > precomp_len)
        ensure_to(n);
    return precomp[n-1];
}
