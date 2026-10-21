/*
 * A071415  Maximal m such that all numbers in [0,m] are expressible as a*b +
 *          c with a + c <= n, b <= n and a,b,c nonnegative integers.
 * https://oeis.org/A071415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 271, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes OEIS sequence A071415:
   a(n) = maximal m such that all numbers in [0,m] are expressible as
          a*b + c with a,c ≥ 0, a + c ≤ n, b ≤ n.
   The algorithm builds a difference array of all intervals
   [a*b , a*b + n - a] for all 0 ≤ a,b ≤ n, then scans the prefix sum
   to find the first gap.  Complexity is O(n^2) time and O(n^2) memory.
   For n ≤ 10000 the memory usage is about 400 MB (int32_t array)
   and the running time is well below 2 s on modern hardware.
*/ 
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The function must have external linkage (not static) */
seq_t a(idx_t n)
{
    /* Handle the trivial case when n == 0 (outside the OEIS offset) */
    if (n == 0) {
        return 0;
    }

    /* Maximum possible value that can be formed is n*n   */
    idx_t max_m = n * n;
    size_t array_len = (size_t)max_m + 2;          /* +2 for diff[end+1] access   */

    /* Allocate difference array and initialise to zero */
    int32_t *diff = (int32_t *)calloc(array_len, sizeof(int32_t));
    if (!diff) {
        /* In case of out‑of‑memory, return 0 as a safe fallback   */
        return 0;
    }

    /* Build the difference array: for each (a,b) add +1 at start
       and -1 at end+1 where the interval is [a*b, a*b + n - a]   */
    for (idx_t a = 0; a <= n; ++a) {
        idx_t rest = n - a;          /* n - a  */
        idx_t start = 0;             /* a*b, starting with b = 0   */
        for (idx_t b = 0; b <= n; ++b) {
            idx_t end = start + rest;          /* inclusive end of interval   */
            diff[start] += 1;
            diff[end + 1] -= 1;
            start += a;                         /* next start = a*(b+1)        */
        }
    }

    /* Scan the prefix sum of the difference array to find the first gap. */
    seq_t cumulative = 0;
    for (idx_t m = 0; m <= max_m; ++m) {
        cumulative += diff[m];
        if (cumulative == 0) {              /* m is missing                  */
            free(diff);
            return (seq_t)(m - 1);           /* maximal m with all numbers < m  */
        }
    }

    /* If all numbers up to n*n are covered (unlikely), return the max. */
    free(diff);
    return (seq_t)max_m;
}
