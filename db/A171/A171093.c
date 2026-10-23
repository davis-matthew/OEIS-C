/*
 * A171093  a(n) is the string of integers S(n) concatenated together with no
 *          commas, where S(1)=1; for even n, S(n) = S(n-1),n; and for odd
 *          n>1, S(n) is obtained by adding 1 to every entry of S(n-1) and
 *          appending 1.
 * https://oeis.org/A171093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = concatenation of the n elements of S(n) where
 *   S(1)   = [1];
 *   if n is even: S(n) = S(n‑1) concatenated with n;
 *   if n is odd > 1: S(n) = S(n‑1) with every entry incremented by 1, then
 *                    appended with 1.
 * The implementation computes each element directly:
 *   let O(n) = number of odd integers ≤ n  = (n+1)/2;
 *   for 1 ≤ j ≤ n,
 *       start = (j%2==0) ? j : 1;
 *       inc   = O(n) – O(j);
 *       v_j = start + inc;
 *   The string of all v_j (in order) is returned as a seq_t via seq_from_str.
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 0) {          /* not used in OEIS indexing, but harmless */
        return seq_from_str("0");
    }

    /* Reserve a buffer that is big enough for roughly 6 digits per element.
       For n up to 10 000 the maximum value is about 15 000, so 6 digits
       are sufficient. */
    size_t bufcap = 6 * (size_t)n + 10;
    char *buf = (char *)malloc(bufcap);
    if (!buf) {
        return seq_from_str("0");
    }
    char *p = buf;

    /* Number of odd indices up to a given m.  Integer division works. */
    uint64_t odds_n = (n + 1) / 2;

    for (uint64_t j = 1; j <= n; ++j) {
        uint64_t odds_j = (j + 1) / 2;
        uint64_t inc    = odds_n - odds_j;
        uint64_t start  = (j % 2 == 0) ? j : 1ULL;
        uint64_t v      = start + inc;

        /* Append v to the buffer.  We use a small temporary buffer
           so that snprintf does not need a destination length. */
        char tmp[32];
        int len = snprintf(tmp, sizeof(tmp), "%llu", (unsigned long long)v);
        if (len < 0) {
            free(buf);
            return seq_from_str("0");
        }

        /* Grow buffer if necessary. */
        if ((size_t)(p - buf) + (size_t)len + 1 > bufcap) {
            size_t newcap = bufcap * 2;
            char *newbuf = (char *)realloc(buf, newcap);
            if (!newbuf) {
                free(buf);
                return seq_from_str("0");
            }
            p = newbuf + (p - buf);
            buf = newbuf;
            bufcap = newcap;
        }

        memcpy(p, tmp, (size_t)len);
        p += len;
    }
    *p = '\0';

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
