/*
 * A104255  a(n) = floor( (2n-1)!!/(2n) ).
 * https://oeis.org/A104255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (2n-1)!! / (2n) ) where (2n-1)!! =
 *            product_{k=1..n} (2k-1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine keeps a running product in an unsigned __int128
   to avoid overflow for the range of n that produce values
   fitting into a 64‑bit seq_t.  For arbitrary‑precision seq_t
   the cast will widen automatically.  The algorithm is O(n). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)                /* offset behaviour is irrelevant */
        return 0;

    /* static state so repeated calls stay O(1) for max(n) */
    static idx_t          computed = 0;    /* maximum n computed so far   */
    static unsigned __int128 cur_prod = 1; /* (2k-1)!! stored as 128‑bit */
    static seq_t           *arr      = NULL;
    static idx_t           cap      = 0;    /* allocation capacity       */

    /* initialise on first call */
    if (computed == 0) {
        cap = (n + 1 > 2) ? n + 1 : 2;
        arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!arr)
            return 0;            /* allocation failed – safe fallback */
        arr[0] = 0;              /* unused dummy entry */
    }

    /* grow the array if required */
    if (n >= cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;            /* allocation failed – safe fallback */
        arr = tmp;
        cap = new_cap;
    }

    /* extend the running product and compute terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        cur_prod *= (unsigned __int128)(2 * k - 1);          /* product (2k-1)!! */
        arr[k] = (seq_t)(cur_prod / (unsigned __int128)(2 * k)); /* floor division */
    }

    computed = n;
    return arr[n];
}
