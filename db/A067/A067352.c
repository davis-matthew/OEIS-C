/*
 * A067352  Divide the natural numbers in sets of consecutive numbers starting
 *          with {1,2} as the first set. The number of elements of the n-th
 *          set is equal to the sum of the n-1 final numbers in the (n-1)st
 *          set. The number of elements of the n-th set gives a(n).
 * https://oeis.org/A067352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1 + a(1)+a(2)+...+a(n-1) - n/2) * (n-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoized cache of computed terms */
    static idx_t      max_n    = 0;      /* highest n computed so far          */
    static size_t     capacity = 0;      /* allocated size for array           */
    static seq_t      *arr      = NULL;  /* array of a(1)..a(max_n)            */
    static seq_t      cum       = 0;      /* prefix sum a(1)+...+a(max_n)      */

    if (n == 0)
        return 0;                       /* offset 1: a(1) is the first term */

    /* initialise cache for n >= 1 */
    if (max_n == 0) {
        capacity = 1;
        arr = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!arr) return 0;             /* out of memory – return 0 as guard */
        arr[0]   = (seq_t)2;            /* a(1) = 2 */
        max_n    = 1;
        cum      = arr[0];
    }

    /* if already computed, just return it */
    if (n <= max_n)
        return arr[n - 1];

    /* grow array if necessary */
    if ((size_t)n > capacity) {
        size_t newcap = capacity;
        if (newcap == 0) newcap = 1;
        while (newcap < (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(arr, newcap * sizeof(seq_t));
        if (!tmp) return 0;          /* out of memory – return 0 as guard */
        arr = tmp;
        capacity = newcap;
    }

    /* compute all terms from max_n+1 up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t T   = (seq_t)(2 * (cum + 1) - i);          /* T = 2*(cum+1)-i   */
        seq_t val = ((seq_t)(i - 1) * T) / 2;            /* a(i) = (i-1)*T/2   */
        arr[i - 1] = val;
        cum += val;
    }
    max_n = n;
    return arr[n - 1];
}
