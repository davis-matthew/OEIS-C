/*
 * A168901  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168901:  number of reduced words of length n in the
 * Coxeter group on 32 generators with (S_i)^2 = (S_i S_j)^21 = I.
 *
 * Recurrence (for n ≥ 22):
 *     a(n) = 30 * (a(n-1)+a(n-2)+ … + a(n-20)) – 465 * a(n-21)
 *
 * Initial values (derived from the generating function)
 * a(0) = 1
 * for 1 ≤ n ≤ 20:
 *     a(n) = 2 + 30 * (a(n-1)+ … + a(0))
 * a(21) = 1 + 30 * (a(20)+ … + a(1)) – 465 * a(0)
 *
 * The function memoises all computed terms so that arbitrary call
 * order or many calls remain fast.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state ----------------------------------------------------- */
    static seq_t *arr = NULL;     /* memoised terms */
    static idx_t cap  = 0;        /* current capacity of arr      */
    static idx_t last = 0;        /* largest index already computed */

    /* helper: grow the array so that arr[0..n] exists */
    if (n >= cap) {
        idx_t new_cap = (cap == 0 ? 64 : cap << 1);
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory: fail safely */
        arr = tmp;
        cap = new_cap;
    }

    /* initialise first term if needed ------------------------------- */
    if (last == 0 && arr[0] == 0) {     /* assumes arr[0] is 0 on first call */
        arr[0] = (seq_t)1;
    }

    /* compute missing terms up to n --------------------------------- */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = 0;

        if (i == 0) {
            /* a(0) already set to 1; nothing to do */
            continue;
        }
        else if (i <= 20) {
            /* p_i = 2 for 1 ≤ i ≤ 20 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k) sum += arr[i - k];   /* a[i-1] ... a[0] */
            val = (seq_t)2 + (seq_t)30 * sum;
        }
        else if (i == 21) {
            /* p_21 = 1 */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 20; ++k) sum += arr[21 - k];   /* a[20] ... a[1] */
            val = (seq_t)1 + (seq_t)30 * sum - (seq_t)465 * arr[0];
        }
        else /* i ≥ 22 */ {
            seq_t sum = 0;
            for (idx_t k = 1; k <= 20; ++k) sum += arr[i - k];   /* a[i-1] ... a[i-20] */
            val = (seq_t)30 * sum - (seq_t)465 * arr[i - 21];
        }

        arr[i] = val;
    }

    last = n;
    return arr[n];
}
