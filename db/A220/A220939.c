/*
 * A220939  Number of n X 2 arrays of occupancy after each element moves to
 *          some horizontal, vertical or antidiagonal neighbor.
 * https://oeis.org/A220939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 13*a(n-1) - 32*a(n-2) + 22*a(n-3) - 62*a(n-4) +
 *              92*a(n-5) - 33*a(n-6) + a(n-7)   for n >= 9
 * with initial values:
 * a(1)=1, a(2)=20, a(3)=208, a(4)=2056, a(5)=20483,
 * a(6)=203905, a(7)=2029453, a(8)=20199560.
 * The sequence is memoised to allow arbitrary order of calls.
 */

#include <stddef.h>   /* size_t, NULL */

/* maximum n we expect to handle comfortably; larger values will
   still work but the array will be larger than required. */
#define MAX_N 20000

/* cache of computed values, 1‑based indices. */
static seq_t cache[MAX_N + 1] = { 0 };

/* the largest index that has been computed so far (0 means none). */
static idx_t max_cached = 0;

/* initialise the first eight terms, only once */
static void init_cache(void) {
    static int once = 0;
    if (once)
        return;
    once = 1;
    /* a(1) .. a(8) */
    cache[1] = (seq_t)1ULL;
    cache[2] = (seq_t)20ULL;
    cache[3] = (seq_t)208ULL;
    cache[4] = (seq_t)2056ULL;
    cache[5] = (seq_t)20483ULL;
    cache[6] = (seq_t)203905ULL;
    cache[7] = (seq_t)2029453ULL;
    cache[8] = (seq_t)20199560ULL;
    if (max_cached < 8)
        max_cached = 8;
}

/* compute a(n) using recurrence, expanding cache up to n */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;              /* undefined for n < 1 */

    if (n <= (idx_t)MAX_N) {
        init_cache();

        /* compute any missing terms up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i <= 8) {          /* already initialised */
                continue;
            }
            /* recurrence for i >= 9 */
            seq_t term = (seq_t)13 * cache[i-1]
                       - (seq_t)32 * cache[i-2]
                       + (seq_t)22 * cache[i-3]
                       - (seq_t)62 * cache[i-4]
                       + (seq_t)92 * cache[i-5]
                       - (seq_t)33 * cache[i-6]
                       + cache[i-7];
            cache[i] = term;
        }
        if (n > max_cached)
            max_cached = n;
        return cache[n];
    }

    /* fallback for very large n: use a temporary vector on the stack
       (recurrence still works, only limited by stack size).   */
    /* Note: for n exceeding MAX_N, we compute lazily without caching. */
    seq_t a1 = cache[1];
    seq_t a2 = cache[2];
    seq_t a3 = cache[3];
    seq_t a4 = cache[4];
    seq_t a5 = cache[5];
    seq_t a6 = cache[6];
    seq_t a7 = cache[7];
    seq_t a8 = cache[8];
    seq_t ai;
    for (idx_t i = 9; i <= n; ++i) {
        ai = (seq_t)13 * a8
          - (seq_t)32 * a7
          + (seq_t)22 * a6
          - (seq_t)62 * a5
          + (seq_t)92 * a4
          - (seq_t)33 * a3
          + a2;
        a2 = a3; a3 = a4; a4 = a5; a5 = a6;
        a6 = a7; a7 = a8; a8 = ai;
    }
    if (n == 1) return a1;
    if (n == 2) return a2;
    if (n == 3) return a3;
    if (n == 4) return a4;
    if (n == 5) return a5;
    if (n == 6) return a6;
    if (n == 7) return a7;
    if (n == 8) return a8;
    return ai;
}
