/*
 * A049891  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A049891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049891
   a(1) = 1, a(2) = 1, a(3) = 2
   for n ≥ 4:
   a(n) = (a(1)+a(2)+…+a(n−1)) − a(m)
   where m = 2*n − 2 − 2^(p+1)
   and p is the unique integer with 2^p < n−1 ≤ 2^(p+1)
*/

#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

seq_t a(idx_t n)
{
    /* static memoisation storage */
    static seq_t *vals   = NULL;      /* a(k) */
    static seq_t *prefix = NULL;      /* prefix sums S(k) = a(1)+…+a(k) */
    static size_t cap   = 0;          /* allocated capacity (≥ current len) */
    static size_t len   = 0;          /* maximum n whose a(n) is computed */

    /* initialisation: a(1)=1, a(2)=1, a(3)=2 */
    if (!vals) {
        cap = 4;                           /* at least space for indices 0..4 */
        vals   = (seq_t *)malloc((cap+1) * sizeof(seq_t));
        prefix = (seq_t *)malloc((cap+1) * sizeof(seq_t));
        vals[1] = (seq_t)1;
        vals[2] = (seq_t)1;
        vals[3] = (seq_t)2;
        prefix[0] = (seq_t)0;
        prefix[1] = vals[1];
        prefix[2] = prefix[1] + vals[2];
        prefix[3] = prefix[2] + vals[3];
        len = 3;
    }

    /* if the requested index is beyond what we have computed, extend the cache */
    if ((size_t)n > len) {
        /* grow storage if necessary */
        if ((size_t)n > cap) {
            size_t new_cap = cap == 0 ? 4 : cap;
            while (new_cap < (size_t)n + 1) new_cap <<= 1;
            vals   = (seq_t *)realloc(vals,   (new_cap+1) * sizeof(seq_t));
            prefix = (seq_t *)realloc(prefix, (new_cap+1) * sizeof(seq_t));
            cap = new_cap;
        }

        /* compute a(k) for all k in (len, n] */
        for (size_t k = len + 1; k <= (size_t)n; ++k) {
            /* p = floor(log2(k-2)) because 2^p < k-1 ≤ 2^(p+1) */
            size_t p = 63 - __builtin_clzll(k - 2);
            size_t m = 2 * k - 2 - ((size_t)1 << (p + 1));

            /* a(k) = S(k-1) - a(m) */
            seq_t ak = prefix[k - 1] - vals[m];
            vals[k] = ak;
            prefix[k] = prefix[k - 1] + ak;
        }
        len = (size_t)n;
    }

    return vals[(size_t)n];
}
