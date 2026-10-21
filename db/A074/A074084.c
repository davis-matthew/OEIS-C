/*
 * A074084  Coefficient of q^1 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(2,1).
 * https://oeis.org/A074084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4), for n>=5 with
 *            initial values a(0)=0, a(1)=0, a(2)=0, a(3)=2, a(4)=9 (see OEIS
 *            A074084)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;       /* cached values */
static idx_t A_len = 0;       /* number of elements stored (size of A) */

/* Ensure that A[n] is available (and all earlier values) */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= A_len) return;          /* already have it */

    /* grow the array */
    seq_t *newA = NULL;
    size_t new_len = (size_t)(n + 1);
    if (A_len == 0) {
        newA = (seq_t *)malloc(new_len * sizeof(seq_t));
    } else {
        newA = (seq_t *)realloc(A, new_len * sizeof(seq_t));
    }
    if (!newA) {
        /* allocation failed – aborting */
        exit(EXIT_FAILURE);
    }
    A = newA;
    /* fill new entries */
    for (idx_t i = A_len; i <= n; ++i) {
        if (i == 0) {
            A[i] = (seq_t)0;
        } else if (i == 1 || i == 2) {
            A[i] = (seq_t)0;
        } else if (i == 3) {
            A[i] = (seq_t)2;
        } else if (i == 4) {
            A[i] = (seq_t)9;
        } else {
            /* recurrence for i >= 5 */
            seq_t t1 = 4 * A[i-1];
            seq_t t2 = 2 * A[i-2];
            seq_t t3 = 4 * A[i-3];
            seq_t t4 = A[i-4];
            A[i] = t1 - t2 - t3 - t4;
        }
    }
    A_len = new_len;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return A[n];
}
