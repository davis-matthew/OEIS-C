/*
 * A075116  Binomial transform of A073817:
 *          a(n)=Sum(Binomial(n,k)*A073817(k),(k=0,..,n)).
 * https://oeis.org/A075116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 8*a(n-2) + 6*a(n-3) - a(n-4), a(0)=4, a(1)=5,
 *            a(2)=9, a(3)=23
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th term of OEIS sequence A075116.  The function
   memoises already computed terms in a static array so that
   arbitrary‑order calls are efficient. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* small constant terms */
    if (n < 4)
        switch (n) {
        case 0: return (seq_t)4;
        case 1: return (seq_t)5;
        case 2: return (seq_t)9;
        case 3: return (seq_t)23;
        }

    /* static cache of previously computed terms */
    static seq_t *cache = NULL;      /* array of a[0..cached_to] */
    static idx_t cached_to = (idx_t)-1; /* last index stored in cache */

    /* initialise cache with first four values on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (cache == NULL)
            abort();    /* allocation failure, unlikely in test env */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)5;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)23;
        cached_to = 3;
    }

    /* extend cache up to n if necessary */
    if (n > cached_to) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            abort();
        cache = tmp;
        for (idx_t i = cached_to + 1; i <= n; ++i)
            cache[i] = 5 * cache[i - 1]
                     - 8 * cache[i - 2]
                     + 6 * cache[i - 3]
                     - cache[i - 4];
        cached_to = n;
    }

    return cache[n];
}
