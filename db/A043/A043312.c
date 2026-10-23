/*
 * A043312  a(n) = A033006(n)/9.
 * https://oeis.org/A043312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑64 representation uses
 *            only the digits 0–7 and contains no pair of adjacent equal
 *            digits. The returned value is this integer itself (dividing by 9
 *            is implicit in the OEIS definition)
 * Range:     u8 n <= 28, u16 n <= 399, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but guard for safety */

    /* Find the length L of the base‑64 number that contains the n‑th term. */
    idx_t   L   = 1;                 /* current length */
    seq_t   pow = 7;                 /* 7^L */
    idx_t   cum = 0;                 /* cumulative count of shorter lengths */

    while (cum + (idx_t)pow < n) {   /* look for first L with cum+7^L >= n */
        cum += (idx_t)pow;
        ++L;
        pow *= 7;
    }

    /* Offset of the desired number within the block of length L. */
    idx_t   offset = n - cum - 1;     /* 0‑based */
    seq_t   rem    = offset;          /* work with seq_t for comparisons */

    /* Pre‑compute powers of 7 up to 7^(L-1). */
    seq_t pow7[64];                  /* large enough for any realistic L */
    pow7[0] = 1;
    for (idx_t i = 1; i <= L; ++i) {
        pow7[i] = pow7[i-1] * 7;
    }

    /* Construct the base‑64 digits from most significant to least. */
    seq_t value = 0;
    int    prev  = -1;                /* previous digit (none yet) */

    for (idx_t pos = 0; pos < L; ++pos) {
        int start = (pos == 0) ? 1 : 0;            /* most significant cannot be 0 */
        seq_t count = pow7[L - 1 - pos];           /* number of completions with a given digit */

        for (int d = start; d <= 7; ++d) {
            if (d == prev) continue;             /* no consecutive equal digits */

            if (rem >= count) {
                rem -= count;                    /* skip all numbers starting with d */
            } else {
                value = value * 64 + d;           /* choose digit d and proceed */
                prev  = d;
                break;
            }
        }
    }

    return value;
}
