/*
 * A180774  Number of distinct solutions of sum{i=1..3}(x(2i-1)*x(2i)) = 0
 *          (mod n), with x() only in 1..n-1.
 * https://oeis.org/A180774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   count unordered triples of unordered pairs (a,b) with 1≤a≤b≤n‑1
 *            such that (a·b + c·d + e·f) ≡ 0 (mod n). The result is the
 *            number of distinct solutions as listed in OEIS A180774
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 179, u64 n <= 447,
 *            u128 n <= 447, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                     /* for size_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array() */
#include "../utils/free_seq_array.h"    /* free_seq_array() */

seq_t a(idx_t n)
{
    /* Residue counts for unordered pairs (i,j) with 1 ≤ i ≤ j < n.  */
    seq_t *c = alloc_seq_array(n);      /* zero‑initialised */

    /* Build c[ residue ] = number of unordered pairs giving that residue.  */
    for (idx_t i = 1; i < n; ++i)
    {
        for (idx_t j = i; j < n; ++j)
        {
            idx_t r = (i * j) % n;     /* 0 ≤ r < n */
            ++c[r];
        }
    }

    seq_t ans = 0;

    /* Count unordered multisets {p,q,r} of pairs satisfying the sum condition.  */
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t ci = c[i];
        if (ci == 0) continue;

        for (idx_t j = i; j < n; ++j)
        {
            seq_t cj = c[j];
            if (cj == 0) continue;

            idx_t sum = i + j;
            idx_t k   = (n - (sum % n)) % n;   /* k ≡ -(i+j) (mod n)   */
            if (k < j) continue;               /* enforce i ≤ j ≤ k        */

            seq_t ck = c[k];
            if (ck == 0) continue;

            if (k == i && i == j)                 /* all equal */
            {
                /* multisets of size 3 from ci items */
                ans += (ci * (ci + 1) * (ci + 2)) / 6;
            }
            else if (i == j)                      /* i = j < k         */
            {
                /* choose 2 from ci with repetition, times ck */
                ans += ((ci * (ci + 1)) / 2) * ck;
            }
            else if (j == k)                      /* i < j = k         */
            {
                /* choose 2 from cj with repetition, times ci */
                ans += ci * ((cj * (cj + 1)) / 2);
            }
            else                                  /* all distinct      */
            {
                ans += ci * cj * ck;
            }
        }
    }

    free_seq_array(c);
    return ans;
}
