/*
 * A014420  Minimal number of Catalan numbers that sum to n.
 * https://oeis.org/A014420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(n)=1 + a(n-A081290(n)) for n>0 where A081290(n) is the
 *            largest Catalan number ≤ n
 * Range:     u8 n <= 212, u16 n <= 9247, u32 n <= 9247, u64 n <= 9247,
 *            u128 n <= 9247, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case */
    if (n == 0)
        return 0;

    /* Count of Catalan summands */
    seq_t count = 0;

    /* Static cache of Catalan numbers */
    static bool init = false;
    static seq_t *cat_vals = NULL;
    static size_t   cat_cnt  = 0;
    static size_t   cat_cap  = 0;

    if (!init) {
        /* Start with C0 = 1 */
        cat_cap = 16;
        cat_vals = (seq_t *)malloc(cat_cap * sizeof(seq_t));
        if (!cat_vals) return 0;  /* Out of memory: return 0 as fallback */
        cat_vals[0] = 1;
        cat_cnt = 1;
        init = true;
    }

    /* Greedy representation: repeatedly subtract the largest Catalan <= n */
    while (n > 0) {
        /* Ensure cache has all Catalan numbers up to n */
        while (true) {
            if (cat_cnt == cat_cap) {
                size_t new_cap = cat_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cat_vals, new_cap * sizeof(seq_t));
                if (!tmp) return count;  /* Out of memory */
                cat_vals = tmp;
                cat_cap = new_cap;
            }
            seq_t last = cat_vals[cat_cnt - 1];
            if (last > n)
                break;  /* Already have all Catalan ≤ n */
            /* Compute next Catalan number:
               C_{k+1} = C_k * 2*(2k+1) / (k+2)   where k = cat_cnt-1 */
            idx_t   k    = cat_cnt - 1;
            seq_t   prev = cat_vals[k];
            seq_t   next = prev * (seq_t)(2 * (2 * k + 1)) / (seq_t)(k + 2);
            if (next > n)
                break;  /* No need to store it; all subsequent will be > n */
            cat_vals[cat_cnt] = next;
            cat_cnt++;
        }

        /* Binary search the largest Catalan ≤ n */
        size_t lo = 0, hi = cat_cnt - 1;
        size_t best = 0;
        while (lo <= hi) {
            size_t mid = lo + (hi - lo) / 2;
            if (cat_vals[mid] <= n) {
                best = mid;
                lo = mid + 1;
            } else {
                if (mid == 0)
                    break;
                hi = mid - 1;
            }
        }

        /* Subtract that Catalan */
        n -= (idx_t)cat_vals[best];
        count++;  /* One summand used */
    }

    return count;
}
