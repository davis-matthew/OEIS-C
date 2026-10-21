/*
 * A228580  y-values in the solution to the Pell equation x^2 - 106*y^2 = -1.
 * https://oeis.org/A228580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A228580 – y-values in the solution to x^2 – 106*y^2 = –1
   Recurrence: a(1) = 389, a(2) = 24958279289
                a(n) = 64160102*a(n‑1) – a(n‑2)  for n ≥ 3
   Offset: 1
   All terms are multiples of 389. */

#include <stddef.h>   /* for size_t */

#define A228580_MAX_N 10010   /* sufficient for n ≤ 10000 */

seq_t a(idx_t n)
{
    /* n == 0 is outside the defined offset – return 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* Static cache of previously computed terms.
       The first two terms are stored at indices 1 and 2. */
    static seq_t cache[A228580_MAX_N];
    static size_t last_computed = 0;      /* highest n that has been computed */

    /* Initialise the first two terms once. */
    if (last_computed == 0) {
        cache[1] = (seq_t)389u;
        cache[2] = (seq_t)24958279289ULL;
        last_computed = 2;
    }

    /* Compute any missing terms up to n using the recurrence. */
    while (last_computed < (size_t)n) {
        /* a(k) = 64160102*a(k-1) - a(k-2)  */
        seq_t const C = 64160102u;
        cache[last_computed + 1] = C * cache[last_computed] - cache[last_computed - 1];
        ++last_computed;
    }

    return cache[(size_t)n];
}
