/*
 * A189437  Number of 5 X n array permutations with each element not moving,
 *          or moving one space N, SW or SE.
 * https://oeis.org/A189437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2) - 47*a(n-3) + 138*a(n-4) -
 *            75*a(n-5) Initial values: a(1)=1, a(2)=20, a(3)=109, a(4)=841,
 *            a(5)=5680
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* Undefined for n < 1; returns 0 */

    static seq_t *cache   = NULL;   /* array storing computed values (1‑based) */
    static size_t max_done = 0;     /* highest index computed so far */
    static size_t capacity = 0;     /* allocated array size */

    /* Expand storage if necessary */
    if ((size_t)n > capacity) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failure – return 0 (undefined behaviour otherwise) */
            return (seq_t)0;
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    if (max_done < (size_t)n) {
        static const seq_t base[5] = {
            (seq_t)1,
            (seq_t)20,
            (seq_t)109,
            (seq_t)841,
            (seq_t)5680
        };

        for (size_t i = max_done + 1; i <= (size_t)n; ++i) {
            if (i <= 5) {
                cache[i - 1] = base[i - 1];
            } else {
                seq_t a1 = cache[i - 2]; /* a(n-1) */
                seq_t a2 = cache[i - 3]; /* a(n-2) */
                seq_t a3 = cache[i - 4]; /* a(n-3) */
                seq_t a4 = cache[i - 5]; /* a(n-4) */
                seq_t a5 = cache[i - 6]; /* a(n-5) */

                /* Compute a(n) using a separate positive and negative accumulator
                   to avoid underflow on unsigned seq_t types. */
                seq_t pos = (seq_t)0;
                seq_t neg = (seq_t)0;
                pos += (seq_t)10 * a1;
                pos += (seq_t)138 * a4;
                neg += (seq_t)17 * a2;
                neg += (seq_t)47 * a3;
                neg += (seq_t)75 * a5;
                cache[i - 1] = pos - neg;
            }
        }
        max_done = n;
    }

    return cache[n - 1];
}
