/*
 * A058218  Positive integers that cannot be represented in the form
 *          n=5|ab|+a+b for any choice of nonzero integers a and b (positive
 *          or negative).
 * https://oeis.org/A058218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 6750, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A058218: Positive integers that cannot be represented in the
   form n = 5|a·b| + a + b for any non‑zero integers a and b.  The
   function returns the n‑th term (n ≥ 1).  The algorithm searches in
   increasing order of the candidate numbers, generating a bitmap of
   all representable numbers up to the current limit, and counting the
   excluded numbers until the desired index is reached.  The bitmap
   and the search limit are increased until the count reaches n.  This
   method is O(L·log L) where L is the numerical value of the n‑th
   excluded term, easily fast enough for n up to about 10 000. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, not used by OEIS */

    /* Initial search limit – enough to contain many terms of the
       excluded list.  Increase it if necessary. */
    uint64_t lim = n * 8 + 100;
    if (lim < 200) lim = 200;

    while (1) {
        /* Allocate bitmap: rep[i] == 1  iff i is representable. */
        uint8_t *rep = (uint8_t*)malloc((size_t)(lim + 1));
        if (!rep) abort();
        memset(rep, 0, (size_t)(lim + 1));

        /* Generate all representable numbers ≤ lim. */
        uint64_t max_u = lim;             /* u can never exceed lim */
        for (uint64_t u = 1; u <= max_u; ++u) {
            /* For each u, iterate v until all four candidate values
               exceed lim.  Use 64‑bit signed arithmetic to avoid wrap‑
               around when the expressions are negative. */
            uint64_t base = 5 * u;
            for (uint64_t v = 1; ; ++v) {
                int64_t val1 = (int64_t)(base * v + u + v);
                if (val1 > 0 && (uint64_t)val1 <= lim)
                    rep[(size_t)val1] = 1;

                int64_t val2 = (int64_t)(base * v + u - v);
                if (val2 > 0 && (uint64_t)val2 <= lim)
                    rep[(size_t)val2] = 1;

                int64_t val3 = (int64_t)(base * v - u + v);
                if (val3 > 0 && (uint64_t)val3 <= lim)
                    rep[(size_t)val3] = 1;

                int64_t val4 = (int64_t)(base * v - u - v);
                if (val4 > 0 && (uint64_t)val4 <= lim)
                    rep[(size_t)val4] = 1;

                if (val1 > (int64_t)lim && val2 > (int64_t)lim &&
                    val3 > (int64_t)lim && val4 > (int64_t)lim)
                    break;  /* all four are already beyond lim */
            }
        }

        /* Scan the bitmap, counting excluded numbers. */
        size_t count = 0;
        seq_t result = 0;
        for (size_t num = 1; num <= lim; ++num) {
            if (!rep[num]) {
                ++count;
                if (count == n) {
                    result = (seq_t)num;
                    free(rep);
                    return result;
                }
            }
        }
        free(rep);

        /* Not enough excluded terms – expand the limit and try again. */
        lim <<= 1;   /* double the limit */
    }
}
