/*
 * A183113  Magnetic Tower of Hanoi, number of moves of disk number k,
 *          optimally solving the [RED ; NEUTRAL ; BLUE] pre-colored puzzle.
 * https://oeis.org/A183113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A183113: Magnetic Tower of Hanoi (disk‑k moves)
 *
 * Recurrence (OEIS entry):
 *     a(0) = 0
 *     a(1) = 1
 *     a(2) = 3
 *     a(3) = 7
 *     a(k) = a(k-2) + 2*a(k-3) + 4*3^(k-3) + 4*3^(k-4)   for k >= 4
 */

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Handle the first four values directly for small n. */
    if (n < 4) {
        return (n == 0) ? (seq_t)0 :
               (n == 1) ? (seq_t)1 :
               (n == 2) ? (seq_t)3 :
                          (seq_t)7;
    }

    /* Static memoisation tables. */
    static seq_t *a_cache = NULL;   /* a(k) values            */
    static size_t a_cap   = 0;      /* capacity of a_cache    */
    static idx_t   a_last  = 3;     /* highest index computed */

    static seq_t *p3_cache = NULL; /* 3^i values (i >= 0)   */
    static size_t p3_cap   = 0;     /* capacity of p3_cache   */

    /* Ensure a_cache can hold index n. */
    if (a_cap <= n) {
        size_t new_cap = n + 1;                      /* up to index n inclusive */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failure        */
        if (a_cap == 0) {                           /* first allocation          */
            tmp[0] = (seq_t)0;
            tmp[1] = (seq_t)1;
            tmp[2] = (seq_t)3;
            tmp[3] = (seq_t)7;
        }
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* Ensure p3_cache can hold powers up to 3^(n-3). */
    idx_t need_p3 = n - 3;                          /* maximum exponent needed   */
    if (p3_cap <= need_p3) {
        size_t new_cap = need_p3 + 1;               /* powers 0 .. need_p3       */
        seq_t *tmp = (seq_t *)realloc(p3_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        if (p3_cap == 0) {                         /* first allocation          */
            tmp[0] = (seq_t)1;                     /* 3^0 = 1                   */
            p3_cap = 1;
        }
        /* Compute new powers of 3. */
        for (idx_t i = p3_cap; i <= need_p3; ++i) {
            tmp[i] = tmp[i - 1] * (seq_t)3;
        }
        p3_cache = tmp;
        p3_cap   = new_cap;
    }

    /* Compute values up to the requested index if not already done. */
    idx_t start = (a_last < 3) ? 4 : a_last + 1;
    for (idx_t k = start; k <= n; ++k) {
        seq_t term = a_cache[k - 2]
                   + (seq_t)2 * a_cache[k - 3]
                   + (seq_t)4 * p3_cache[k - 3]
                   + (seq_t)4 * p3_cache[k - 4];
        a_cache[k] = term;
    }
    a_last = n;

    return a_cache[n];
}
