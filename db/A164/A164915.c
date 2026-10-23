/*
 * A164915  Inverse of binomial matrix (10^n,1) A164899. (See A164899 for
 *          companion sequence.)
 * https://oeis.org/A164915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 65, i64 n <= 209, i128 n <= 819,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array, seq_mul, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A(r,c) where
    let d be the smallest integer with d(d+1)/2 ≥ n;
    let k = n - (d-1)d/2      (1‑based shift within the d‑th anti‑diagonal);
    then  r = d - k + 1 ,  c = k.
    A(r,c) satisfies A(1,c)=10^(c-1), A(r,1)=1 and
    A(r,c)=A(r-1,c)-A(r-1,c-1).  This is equivalent to

          A(r,c)=∑_{j=0}^{min(r-1,c-1)} (-1)^j·C(r-1,j)·10^{c-j-1}   */

/*  No external static objects beyond the small cache shown below.  */
/*  The routine works for any seq_t type supported by the build.  */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"

static seq_t *pow10 = NULL;        /* array of 10^e for e = 0 .. pow10_len-1 */
static size_t pow10_len = 0;       /* current length of pow10 (>=1 after init) */

/* initialise or grow the powers of ten cache to at least 'need_exp' (inclusive) */
static void ensure_pow10(size_t need_exp)
{
    if (pow10_len > need_exp) return;

    size_t new_len = need_exp + 1;
    seq_t *new_arr = seq_alloc_new_array(new_len);

    /* copy old entries */
    for (size_t i = 0; i < pow10_len; ++i)
        new_arr[i] = pow10[i];

    /* compute new entries */
    for (size_t i = pow10_len; i < new_len; ++i) {
        if (i == 0)
            new_arr[0] = seq_one();
        else
            new_arr[i] = seq_mul(new_arr[i - 1], (seq_t)10);
    }

    seq_free_array(pow10);
    pow10 = new_arr;
    pow10_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety guard, offset starts at 1 */

    /* locate the anti‑diagonal that contains the n‑th element   */
    idx_t d = 1;
    while ((d * (d + 1u) / 2u) < n) ++d;          /* minimal d with d(d+1)/2 ≥ n */

    idx_t prev_sum = (d - 1u) * d / 2u;
    idx_t k = n - prev_sum;                       /* 1‑based offset within diagonal */
    idx_t r = d - k + 1u;                         /* row number (A(r,c)) */
    idx_t c = k;                                  /* column number */

    /* prepare powers of 10 up to exponent c-1 */
    if (c == 0) return seq_one();                /* safety, never happens for n≥1 */
    ensure_pow10(c - 1);

    /* compute A(r,c) by the closed formula */
    seq_t result = (seq_t)0;
    seq_t binom  = seq_one();                    /* C(r-1,0) */
    int sign = 1;                                 /* (-1)^j */

    idx_t lim = (r - 1u < c - 1u) ? (r - 1u) : (c - 1u);
    for (idx_t j = 0; j <= lim; ++j) {
        seq_t term = seq_mul(binom, pow10[c - j - 1u]);

        if (sign > 0)
            result = result + term;
        else
            result = result - term;

        if (j == lim) break;                     /* no next binomial needed */

        /* update binomial coefficient for next j   */
        binom = (seq_mul(binom, (seq_t)(r - 1u - j))) / (seq_t)(j + 1u);

        sign = -sign;
    }

    return result;
}
