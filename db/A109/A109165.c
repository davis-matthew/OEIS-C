/*
 * A109165  a(n) = 5*a(n-2) - 2*a(n-4), n >= 4.
 * https://oeis.org/A109165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:  A109165
   Recurrence:  a(n) = 5*a(n-2) - 2*a(n-4)   (n ≥ 4)
   Initial values:  a(0)=1, a(1)=2, a(2)=5, a(3)=10   (offset 0) */

#include <stdlib.h>

static seq_t *A = NULL;         /* cache of computed terms */
static idx_t  cap = 0;          /* current capacity (number of slots) */
static idx_t  len = 0;          /* number of values actually filled */

/* Ensure that the array A has at least (n+1) slots. */
static void
ensure_capacity(idx_t n)
{
    if (n < cap)
        return;
    idx_t new_cap = cap ? cap * 2 : 16;
    while (new_cap <= n)
        new_cap <<= 1;          /* grow until new_cap > n */
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                /* out of memory */
    A = tmp;
    cap = new_cap;
}

seq_t
a(idx_t n)
{
    if (n < len)               /* already computed */
        return A[n];

    ensure_capacity(n);

    /* Compute missing values from the current length up to n. */
    for (idx_t i = len; i <= n; ++i) {
        if (i < 4) {           /* initial values */
            switch (i) {
                case 0: A[0] = (seq_t)1; break;
                case 1: A[1] = (seq_t)2; break;
                case 2: A[2] = (seq_t)5; break;
                case 3: A[3] = (seq_t)10; break;
            }
        } else {               /* recurrence */
            A[i] = (5 * A[i-2]) - (2 * A[i-4]);
        }
    }
    len = n + 1;              /* update length of computed data */
    return A[n];
}
