/*
 * A202349  Lexicographically earliest sequence such that the sequence and its
 *          first and second differences share no terms, and the 3rd
 *          differences are equal to the original sequence.
 * https://oeis.org/A202349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) Initial values: a(1)=1,
 *            a(2)=3, a(3)=9
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the offset: a(1) is the first term.  n==0 is unused. */
    if (n == 0) return 0;

    static seq_t *cache = NULL;   /* array of already computed terms */
    static idx_t cap   = 0;       /* current allocation size   */
    static idx_t len   = 0;       /* number of terms stored    */

    /* Initialise the first three terms when needed */
    if (len == 0) {
        /* allocate at least three slots, grow power‑of‑two */
        cap = 4;
        cache = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!cache) exit(1);          /* allocation failure */

        cache[0] = 1;                 /* a(1) */
        cache[1] = 3;                 /* a(2) */
        cache[2] = 9;                 /* a(3) */
        len = 3;
    }

    /* Return immediately if we already have the requested term */
    if (n <= len) {
        return cache[n - 1];
    }

    /* Grow the cache if necessary */
    if (n > cap) {
        idx_t new_cap = cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = len + 1; i <= n; ++i) {
        /* i is 1‑based; cache stores 0‑based indices */
        seq_t a1 = cache[i - 2];   /* a(i-1) */
        seq_t a2 = cache[i - 3];   /* a(i-2) */
        seq_t a3 = cache[i - 4];   /* a(i-3) */
        cache[i - 1] = 3 * a1 - 3 * a2 + 2 * a3;
    }

    len = n;
    return cache[n - 1];
}
