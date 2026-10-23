/*
 * A181868  a(1) = 10, a(2) = 1. For n >= 3, a(n) = concatenate a(n-1),
 *          a(n-2), ..., a(1) and then divide the resulting number by a(n-1).
 * https://oeis.org/A181868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=10, a(2)=1. For n≥3,
   a(n)=a(n-2)+10^( sumLen(n-2) ),
   where sumLen(k)=∑_{i=1}^{k} digits(a(i)).
   This is equivalent to the OEIS formula a(n+2)=a(n)+10^(A000032(n+1)-1). */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Protect against calls with n == 0 (offset 1). */
    if (n == 0) return (seq_t)0;

    static seq_t *values = NULL;
    static idx_t *sumLen = NULL;
    static idx_t computed = 0;    /* highest index currently cached (1‑based) */
    static size_t cap = 0;        /* current allocation size (in entries) */

    /* Return cached value if available. */
    if (n <= computed) return values[n];

    /* Ensure enough capacity for indices up to n. */
    idx_t needed = n + 1;        /* allocate one spare entry for 0‑unused index */
    if ((size_t)needed > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < (size_t)needed) newcap = (size_t)needed;
        seq_t *tmp = (seq_t *)realloc(values, newcap * sizeof(seq_t));
        if (!tmp) abort();        /* allocation failed */
        values = tmp;
        idx_t *tmp2 = (idx_t *)realloc(sumLen, newcap * sizeof(idx_t));
        if (!tmp2) abort();
        sumLen = tmp2;
        cap = newcap;
    }

    /* Initialise base terms if this is the first call. */
    if (computed == 0) {
        values[1] = (seq_t)10;
        sumLen[1] = 2;             /* digits(a(1)) = 2 */
        values[2] = (seq_t)1;
        sumLen[2] = sumLen[1] + 1; /* digits(a(2)) = 1 */
        computed = 2;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        idx_t exp = sumLen[i - 2];                 /* A000032(n+1)-1 in original formula */
        seq_t term = values[i - 2] + pow10(exp);   /* a(i) = a(i-2) + 10^exp */
        values[i] = term;

        /* digits(a(i)) = exp + 1, because 10^exp has exp+1 digits
           and a(i-2) is strictly smaller than 10^exp. */
        idx_t curLen = exp + 1;
        sumLen[i] = sumLen[i - 1] + curLen;        /* update cumulative digit count */
    }

    computed = n;
    return values[n];
}
