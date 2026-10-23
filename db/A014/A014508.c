/*
 * A014508  a(n) = floor( n! / e ).
 * https://oeis.org/A014508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n! / e)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute floor(n! / e) for n >= 1.
   Uses the derangement recurrence !n = n*!(n-1) + (-1)^n.
   Then a(n) = !n - (n % 2 == 0 ? 1 : 0). */
seq_t a(idx_t n)
{
    /* Static memoisation buffer for derangements !k */
    static seq_t *der = NULL;
    static size_t cap = 0;          /* allocated capacity */
    static seq_t d0, d1;            /* placeholders for d0 and d1 */

    if (!der) {                     /* first call: allocate minimal space */
        cap = 2;                    /* we need indices 0 and 1 at least */
        der = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!der) return 0;         /* memory allocation failed – return 0 */
        der[0] = 1;                 /* !0 = 1 */
        der[1] = 0;                 /* !1 = 0 */
    }

    /* Ensure buffer can store up to index n */
    if (n >= cap) {
        size_t newcap = cap;
        while (n >= newcap)
            newcap = newcap * 2 + 1;          /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(der, newcap * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failed */
        der = tmp;
        cap = newcap;
    }

    /* Compute derangements up to n if necessary */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)i * der[i-1];
        if (i & 1)              /* i odd → subtract 1 */
            term -= 1;
        else                     /* i even → add 1 */
            term += 1;
        der[i] = term;
    }

    /* Compute a(n) from !n */
    seq_t an = der[n];
    if (!(n & 1)) {                 /* n even → subtract 1 */
        an -= 1;
    }
    return an;
}
