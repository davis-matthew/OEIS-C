/*
 * A037542  Base 9 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,2,1.
 * https://oeis.org/A037542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the base‑9 number consisting of the first n digits of the
 *            periodic string 1,2,1,1,2,1,… (digit d_k = 2 if k≡2 (mod 3) else
 *            1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute the n-th term of OEIS A037542. */
seq_t a(idx_t n)
{
    /* Off‑sized index: a(1) is the first element; return 0 for n==0. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache to memoise computed terms. */
    static seq_t *cache = NULL;   /* dynamically allocated array */
    static size_t cap   = 0;      /* current allocation size */
    static size_t sz    = 0;      /* number of terms already computed */

    /* Grow the cache if we need more space. */
    if ((size_t)n > cap) {
        size_t newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)            /* allocation failure → return 0 */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* Compute terms that are not yet present. */
    for (idx_t i = sz + 1; i <= n; ++i) {
        seq_t digit = ((i % 3) == 2) ? (seq_t)2 : (seq_t)1;

        if (i == 1) {
            cache[0] = (seq_t)1;               /* a(1) = 1 */
        } else {
            cache[(size_t)(i - 1)] =
                cache[(size_t)(i - 2)] * (seq_t)9 + digit;
        }
    }

    sz = (size_t)n;                      /* update computed length */
    return cache[(size_t)(n - 1)];
}
