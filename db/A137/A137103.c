/*
 * A137103  Numbers k such that k and k^2 use only the digits 2, 4, 6 and 8.
 * https://oeis.org/A137103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 41,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n‑th term of OEIS A137103.
   The first five terms are hard‑coded:
       1 → 2
       2 → 8
       3 → 22
       4 → 68
       5 → 262
   For n ≥ 6 the term consists of (n‑4) copies of the digit '6' followed
   by a single '8'.  That yields the sequence:
       6 → 668
       7 → 6668
       8 → 66668
       ...
   Clearly all such numbers use only the digits 2,4,6,8 and their squares
   do as well.  The algorithm constructs the decimal string, converts it
   with seq_from_str, and returns the result.
*/

#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)22;
    if (n == 4) return (seq_t)68;
    if (n == 5) return (seq_t)262;

    /* n ≥ 6 : build a string of (n‑4) '6's followed by '8'. */
    size_t n_digits = (size_t)(n - 4) + 1;   /* total digits = n‑3 */
    char *buf = (char *)malloc(n_digits + 1); /* +1 for the null terminator */
    if (!buf) return (seq_t)0;  /* allocation failure → return zero */

    size_t i;
    for (i = 0; i < n - 4; ++i) buf[i] = '6';
    buf[n - 4] = '8';
    buf[n_digits] = '\0';

    seq_t val = seq_from_str(buf);
    free(buf);
    return val;
}
