/*
 * A173184  Partial sums of A000166.
 * https://oeis.org/A173184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of derangements (!k): a(n) = Σ_{k=0}^n !k
   Recurrence: !0 = 1, !1 = 0, !k = (k-1)*( !(k-1)+!(k-2) ) */
#include <stddef.h>
#include <stdlib.h>

static void grow_arr(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap > needed) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap * 2;
    while (new_cap <= needed) new_cap *= 2;
    *arr = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    static seq_t *der = NULL;          /* !k values               */
    static size_t der_cap = 0;
    static size_t der_len = 0;

    static seq_t *psum = NULL;         /* partial sums a(k)       */
    static size_t psum_cap = 0;
    static size_t psum_len = 0;

    /* initialise the first two values */
    if (der_len == 0) {
        grow_arr(&der, &der_cap, 2);
        grow_arr(&psum, &psum_cap, 2);

        der[0] = (seq_t)1;             /* !0 */
        der[1] = (seq_t)0;             /* !1 */
        der_len = 2;

        psum[0] = der[0];              /* a(0) */
        psum[1] = psum[0] + der[1];    /* a(1) */
        psum_len = 2;
    }

    size_t N = (size_t)n;              /* avoid signed/unsigned mix */
    if (N + 1 > psum_len) {            /* need values up to a(n)     */
        size_t target = N + 1;
        grow_arr(&der, &der_cap, target);
        grow_arr(&psum, &psum_cap, target);

        for (size_t k = psum_len; k < target; ++k) {
            seq_t d_k = (seq_t)(k - 1) * (der[k - 1] + der[k - 2]); /* !k */
            der[k] = d_k;
            psum[k] = psum[k - 1] + d_k;                            /* a(k) */
        }
        der_len = psum_len = target;
    }

    return psum[N];
}
