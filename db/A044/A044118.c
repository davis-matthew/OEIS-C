/*
 * A044118  Numbers n such that string 2,4 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑6 representation contains the
 *            substring "24" but n−1 does not. The implementation generates
 *            terms by brute force and stores them in a static array
 *            (std::vector when compiling as C++)
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#  include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    static std::vector<seq_t> vals;
#else
    static seq_t *vals = NULL;
    static size_t cap = 0, cnt = 0;
#endif
    static idx_t last_checked = 0;       /* last integer examined */
    static bool prev_has24 = false;      /* did last integer contain "24"? */

#ifndef __cplusplus
    if (!vals) {
        cap = 128;
        vals = (seq_t*)malloc(cap * sizeof(seq_t));
    }
#endif

    /* Generate terms until we have at least n of them */
    while ((size_t)(n - 1) >=
#ifndef __cplusplus
        cnt
#else
        vals.size()
#endif
    ) {
        ++last_checked;
#ifdef __cplusplus
        seq_t cur = static_cast<seq_t>(last_checked);
#else
        seq_t cur = (seq_t)last_checked;
#endif
        bool cur_has24 = false;

        /* Convert cur to base‑6, least‑significant digit first. */
        seq_t tmp = cur;
        unsigned char digits[128];
        size_t len = 0;

        if (tmp == 0) {
            digits[0] = 0;
            len = 1;
        } else {
            while (tmp != 0) {
                seq_t d = tmp % 6;
                tmp /= 6;
                digits[len++] = (unsigned char)d;
            }
            /* Check for the pattern 2 followed by 4 when the digits
               are read most‑significant first.  With the LSB‑first
               array this is digits[i]==2 && digits[i-1]==4. */
            for (size_t i = 1; i < len; ++i) {
                if (digits[i] == 2 && digits[i - 1] == 4) {
                    cur_has24 = true;
                    break;
                }
            }
        }

        /* Store cur if it meets the criterion while the previous
           number did not. */
        if (cur_has24 && !prev_has24) {
#ifndef __cplusplus
            if (cnt == cap) {
                size_t newcap = cap * 2;
                seq_t *newptr = (seq_t*)realloc(vals, newcap * sizeof(seq_t));
                if (!newptr) return 0;
                vals = newptr;
                cap = newcap;
            }
            vals[cnt++] = cur;
#else
            vals.push_back(cur);
#endif
        }

        prev_has24 = cur_has24;
    }

    /* Return the requested term. */
#ifndef __cplusplus
    return vals[n - 1];
#else
    return vals[n - 1];
#endif
}
