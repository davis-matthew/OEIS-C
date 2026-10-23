/*
 * A167085  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A167085: Number of reduced words of length n in a Coxeter group on 32 generators
 *
 *  The generating function
 *
 *        (t^13 + 2·t^12 + … + 2·t + 1)
 *    G(t) = ------------------------------------
 *               (465·t^13 − 30·t^12 − … − 30·t + 1)
 *
 *  gives the linear recurrence
 *
 *    a_n = 30·a_{n-1} + 30·a_{n-2} + … + 30·a_{n-12}
 *          − 465·a_{n-13}   +  b_n
 *
 *  where the particular part b_n is
 *
 *    b_0 = 1,      b_n = 2   for 1 ≤ n ≤ 12,
 *    b_{13} = 1,   b_n = 0   for n ≥ 14.
 *
 *  For n ≥ 14 the recurrence is homogeneous:
 *
 *    a_n = 30·∑_{k=1}^{12} a_{n-k} − 465·a_{n-13}.
 *
 *  The first 14 values are taken from the OEIS entry and are used as a
 *  base for further computation.  The function is memoised so that
 *  repeated calls are O(1) after the first evaluation up to the largest
 *  argument.
 *
 *  The code uses the helper function seq_from_str to initialise large
 *  constants without overflow.
 *
 * Parameters
 * ----------
 * n : idx_t
 *     Non‑negative index (0‑based).  For n < 0 the behaviour is undefined.
 *
 * Returns
 * -------
 * seq_t
 *     a(n)
 */
#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* --- memoisation state --------------------------------------------- */
    static seq_t *cache = NULL;       /* array of computed terms    */
    static idx_t  max_n  = 0;          /* highest index stored       */
    static int    init   = 0;          /* initialisation flag        */

    /* --- initialise base values (n = 0 … 13) ---------------------------- */
    if (!init) {
        /* Pre‑allocate enough space for the base plus any future indices. */
        size_t init_sz = (size_t)n + 1 > 14 ? (size_t)n + 1 : 14;
        cache = (seq_t *)malloc(init_sz * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */

        /* Decimal strings for the first 14 terms (0 … 13).  We use
         * seq_from_str to avoid overflow of C integer literals.           */
        static const char *const base_str[14] = {
            "1",
            "32",
            "992",
            "30752",
            "953312",
            "29552672",
            "916132832",
            "28400117792",
            "880403651552",
            "27292513198112",
            "846067909141472",
            "26228105183385632",
            "813071260684954592",
            "25205209081233591856"
        };

        for (size_t i = 0; i < 14; ++i)
            cache[i] = seq_from_str(base_str[i]);

        max_n = 13;
        init  = 1;
    }
    /* --------------------------------------------------------------------- */

    /* Return if the requested value is already known */
    if (n <= max_n)
        return cache[n];

    /* Grow cache to accommodate up to index n */
    size_t new_sz = (size_t)n + 1;
    if (new_sz > (size_t)max_n + 1) {
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
    }

    /* ---------------------------------------------------------------------
     *  Generate missing terms using the recurrence.
     *
     *  For n ≥ 14:
     *      a[n] = 30 * (a[n-1] + ... + a[n-12]) - 465 * a[n-13]
     *  For 1 ≤ n ≤ 12:
     *      a[n] = 30 * (a[n-1] + ... + a[0])     - 465 * a[-?] + 2
     *  For n = 13:
     *      a[13] derived from base and satisfies
     *          a[13] = 30 * (a[12] + ... + a[1]) - 465 * a[0] + 1
     *
     *  Since all base cases 0…13 are already stored, we only need the
     *  homogeneous recurrence for i ≥ 14.
     * --------------------------------------------------------------------- */
    for (size_t i = max_n + 1; i <= (size_t)n; ++i) {
        /* Only homogeneous part is needed for i ≥ 14.  Because all
         * lower indices are known, we can safely use the simple loop.   */
        seq_t sum = 0;
        for (int k = 1; k <= 12; ++k)
            sum += cache[i - k];

        cache[i] = (seq_t)30 * sum - (seq_t)465 * cache[i - 13];
    }

    max_n = (idx_t)n;
    return cache[(size_t)n];
}
