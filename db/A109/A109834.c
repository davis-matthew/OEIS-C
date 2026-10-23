/*
 * A109834  Startorial numbers: product of initial digits of integers 1
 *          through n.
 * https://oeis.org/A109834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = Product_{i=1..n} A000030(i) (initial digit of i)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 31, u64 n <= 48, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the most significant decimal digit of x (x >= 1). */
static seq_t first_digit(idx_t x)
{
    while (x >= 10)
        x /= 10;
    return (seq_t)x;          /* convert to seq_t for multiplication */
}

/* Memoized computation of the startorial product. */
seq_t a(idx_t n)
{
    /* No meaningful value for n = 0; caller must use n >= 1. */
    if (n == 0)
        return (seq_t)0;

    static seq_t *prod = NULL;   /* prod[i] stores a(i) */
    static idx_t  cap  = 0;       /* current array capacity */
    static idx_t  sz   = 0;       /* largest index computed */

    if (prod == NULL) {           /* first invocation, prepare array */
        cap = 128;
        sz  = 0;
        prod = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!prod) return (seq_t)0;   /* allocation failure */
        prod[0] = (seq_t)1;           /* a(0) = 1 for convenient recurrence */
    }

    /* Expand capacity if necessary. */
    if (n >= cap) {
        idx_t newcap = cap;
        while (n >= newcap)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prod, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        prod = tmp;
        cap  = newcap;
    }

    /* Compute products for indices not yet calculated. */
    if (n > sz) {
        seq_t last = prod[sz];
        for (idx_t i = sz + 1; i <= n; ++i) {
            last *= first_digit(i);
            prod[i] = last;
        }
        sz = n;
    }

    return prod[n];
}
