/*
 * A073381  Fourth convolution of A000129(n+1) (generalized (2,1)-Fibonacci,
 *          called Pell numbers), n>=0, with itself.
 * https://oeis.org/A073381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 2^(n-2k) * binomial(n-k+4, 4) *
 *            binomial(n-k, k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 37, u128 n <= 84,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* cache of powers of two: pow2[e] = 2^e   */
static seq_t *pow2 = NULL;   /* the array itself   */
static idx_t pow2_len = 0;   /* number of valid entries (pow2[0..pow2_len-1]) */

/* make sure pow2[0..n] exist; silently fail on allocation error */
static void ensure_pow2(idx_t n)
{
    if (pow2_len > n)                /* already ready   */
        return;

    idx_t new_sz = n + 1;            /* need indices 0..n   */
    seq_t *tmp = (seq_t *)realloc(pow2, (size_t)new_sz * sizeof(seq_t));
    if (!tmp)                        /* allocation failed  */
        return;
    pow2 = tmp;

    if (pow2_len == 0) {             /* first allocation   */
        pow2[0] = 1;                 /* 2^0 = 1           */
        pow2_len = 1;
    }

    for (idx_t i = pow2_len; i <= n; ++i)
        pow2[i] = pow2[i - 1] * 2;   /* 2^i = 2 * 2^(i-1)  */

    pow2_len = new_sz;
}

/* -----------------  sequence generator  ----------------- */
seq_t a(idx_t n)
{
    ensure_pow2(n);           /* we need powers up to 2^n  */

    seq_t result = 0;
    idx_t max_k = n >> 1;     /* floor(n/2)   */

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t exp2  = n - (k << 1);         /* n-2k        */
        idx_t t1    = n - k + 4;            /* for binomial(n-k+4,4) */

        seq_t term = pow2[exp2];
        term *= binomial(t1, 4);            /* C(n-k+4,4)  */
        term *= binomial(n - k, k);        /* C(n-k, k)   */

        result += term;
    }

    return result;
}
