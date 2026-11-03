/*
 * A228154  T(n,k) is the number of s in {1,...,n}^n having longest contiguous
 *          subsequence with the same value of length k; triangle T(n,k),
 *          n>=1, 1<=k<=n, read by rows.
 * https://oeis.org/A228154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c), where r is the smallest integer with * r(r+1)/2 ≥
 *            n, c = n − r(r−1)/2, and T(r,c) is the number of * length‑r
 *            strings over an r‑letter alphabet whose longest run of * equal
 *            symbols has length exactly c. T(r,c) = Count(r,c) –
 *            Count(r,c−1), * where Count(r,k) counts strings with long...
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 46, u64 n <= 136,
 *            u128 n <= 378, big any n
 * Validated: standard, pass, 61 terms through n = 9459 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers */
#include <stdio.h>
#include <stdlib.h>
#include "../utils/mem.h"

/*  A helper routine that counts the number of strings of length n
 *  over an n‑letter alphabet whose longest run is at most maxRun.
 *  maxRun == 0 returns 0.  */
static seq_t
count_leq_run(idx_t n, idx_t maxRun)
{
    if (maxRun == 0)
        return 0;

    /*  Allocate arrays for DP.  */
    size_t m = (size_t)maxRun + 1;             /* 1…maxRun, index 0 unused */
    seq_t *prev = mem_new(seq_t, m);
    seq_t *cur  = mem_new(seq_t, m);
    if (!prev || !cur) {
        /*  Memory failure – return 0.  */
        mem_free(prev);
        mem_free(cur);
        return 0;
    }

    prev[1] = (seq_t)n;          /*  length = 1, run = 1 */

    for (idx_t pos = 2; pos <= n; ++pos) {
        seq_t sum = 0;
        for (size_t l = 1; l < m; ++l)
            sum += prev[l];

        cur[1] = (seq_t)(n - 1) * sum;   /*  choose a different symbol   */

        for (size_t l = 2; l < m; ++l)
            cur[l] = prev[l - 1];         /*  extend existing run        */

        /*  Swap buffers :  cur → prev  */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;

        /*  Zero new cur for next iteration.  */
        for (size_t l = 1; l < m; ++l)
            cur[l] = 0;
    }

    seq_t total = 0;
    for (size_t l = 1; l < m; ++l)
        total += prev[l];

    mem_free(prev);
    mem_free(cur);
    return total;
}

/*  The triangle entry T(n,k).  */
static seq_t
T(idx_t n, idx_t k)
{
    if (k == 0 || k > n)
        return 0;
    if (k == n)
        return (seq_t)n;                 /* all symbols equal */

    seq_t a = count_leq_run(n, k);
    seq_t b = count_leq_run(n, k - 1);
    return a - b;
}

/*  The demanded sequence function.  */
seq_t
a(idx_t n)
{
    /*  Find the row r such that r(r+1)/2 ≥ n > (r−1)r/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)  /*  r+1 iterations  */
        ++r;

    /*  Now r satisfies r(r+1)/2 < n ≤ (r+1)(r+2)/2  */
    r++;                                 /*  shift to 1‑based row */

    /*  Column index c  */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t c = n - prev_total;            /*  1 ≤ c ≤ r  */

    return T(r, c);
}
