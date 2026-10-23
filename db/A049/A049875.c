/*
 * A049875  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 2,3,3.
 * https://oeis.org/A049875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-4); a(0)=2, a(1)=3, a(2)=3,
 *            a(3)=6
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A = NULL;          /* cache of computed terms */
static idx_t A_size = 0;         /* allocated size of A */
static idx_t A_max   = 0;        /* highest index computed */
static int   A_init  = 0;        /* whether base terms have been stored */

/* Ensure that the cache can hold indices up to n */
static void ensure_size(idx_t n)
{
    if (A_size <= n) {
        idx_t new_size = (A_size == 0) ? 8 : A_size;
        while (new_size <= n) new_size <<= 1;         /* double size until sufficient */
        seq_t *tmp = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        if (!tmp) abort();                            /* allocation failed */
        A      = tmp;
        A_size = new_size;
    }
}

/* Compute the sequence up to index n (inclusive) */
static void compute_up_to(idx_t n)
{
    if (!A_init) {
        /* initial terms a(0) … a(3) */
        ensure_size(3);                                 /* enough room for early indices */
        A[0] = (seq_t)2;
        A[1] = (seq_t)3;
        A[2] = (seq_t)3;
        A[3] = (seq_t)6;
        A_max = 3;
        A_init = 1;
    }

    if (n <= A_max) return;                          /* already computed */

    for (idx_t i = A_max + 1; i <= n; ++i) {
        A[i] = 2 * A[i - 1] - A[i - 3] + A[i - 4];
    }
    A_max = n;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    compute_up_to(n);
    return A[n];
}
