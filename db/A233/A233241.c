/*
 * A233241  Number of 3Xn 0..5 arrays with no element x(i,j) adjacent to value
 *          5-x(i,j) horizontally, diagonally or antidiagonally, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order.
 * https://oeis.org/A233241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * Sequence A233241 – 3×n arrays
 * 
 * Empirical recurrence (valid for n ≥ 11):
 *   a(n) = 79*a(n-1) - 844*a(n-2) - 956*a(n-3) + 16385*a(n-4)
 *            - 10631*a(n-5) - 45620*a(n-6) + 63092*a(n-7)
 *            - 24384*a(n-8) + 2880*a(n-9)
 *
 *  Initial values (1‑based, a(1)…a(10)):
 *   11, 313, 18744, 1212549, 79794804, 5267525102,
 *   347931872810, 22984197561232, 1518355064369787,
 *   100304170431867107
 *
 *  The function memoises computed terms and works for any
 *  n ≥ 1 in any order of calls.
 */

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* static cache of already computed terms (1‑based) */
    static seq_t *cache = NULL;
    static idx_t cached_upto = 0;   /* highest index stored in cache */

    /* first time: initialise cache with the 10 given terms */
    if (!cache) {
        /* indices 0..10, 0 unused to keep 1‑based indexing */
        size_t init_size = 10 + 1;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (!cache) exit(1);

        /* string representations of a(1)…a(10) */
        static const char *init_str[10] = {
            "11",
            "313",
            "18744",
            "1212549",
            "79794804",
            "5267525102",
            "347931872810",
            "22984197561232",
            "1518355064369787",
            "100304170431867107"
        };

        for (idx_t i = 1; i <= 10; ++i)
            cache[i] = seq_from_str(init_str[i - 1]);

        cached_upto = 10;
    }

    /* if requested value is already cached, return it */
    if (n <= cached_upto)
        return cache[n];

    /* enlarge cache to accommodate indices up to n */
    if (n > cached_upto) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;

        /* compute missing terms using the recurrence */
        for (idx_t i = cached_upto + 1; i <= n; ++i) {
            seq_t val = 0;

            val += (seq_t)79      * cache[i - 1];
            val -= (seq_t)844     * cache[i - 2];
            val -= (seq_t)956     * cache[i - 3];
            val += (seq_t)16385   * cache[i - 4];
            val -= (seq_t)10631   * cache[i - 5];
            val -= (seq_t)45620   * cache[i - 6];
            val += (seq_t)63092   * cache[i - 7];
            val -= (seq_t)24384   * cache[i - 8];
            val += (seq_t)2880    * cache[i - 9];

            cache[i] = val;
        }
        cached_upto = n;
    }

    return cache[n];
}
