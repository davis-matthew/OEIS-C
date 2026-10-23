/*
 * A034832  a(n) = n-th sept-factorial number divided by 5.
 * https://oeis.org/A034832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{j=1..n} (7*j-2) / 5
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (iconically the "sept‑factorial" divided by 5) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache: store partial products prod[i] = Π_{j=1..i}(7*j-2) */
    static seq_t *prod = NULL;       /* array for partial products */
    static idx_t capacity = 0;       /* current allocated size (>= prod+1) */
    static idx_t last = 0;           /* index up to which prod[0..last] are valid */
                                   /* prod[0] is conventionally 1 (empty product) */

    if (n == 0)                 /* safety: the sequence starts at n=1 */
        return 0;

    /* Ensure the buffer is large enough for indices [0..n] */
    if (capacity <= n) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp)                 /* allocation failed – return 0 */
            return 0;
        /* copy existing values that we already computed */
        for (idx_t i = 0; i <= last; ++i)
            tmp[i] = prod ? prod[i] : (i == 0 ? (seq_t)1 : 0);
        free(prod);
        prod = tmp;
        capacity = new_cap;
    }

    /* Compute any missing terms, starting just after the last computed one */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        seq_t term = (seq_t)7 * i - 2;          /* term = 7*i - 2 */
        prod[i] = prod[i - 1] * term;           /* accumulative product */
    }
    last = n;                                  /* update last computed index */

    /* Return the sept‑factorial divided by 5 (integer division) */
    return prod[n] / 5;
}
