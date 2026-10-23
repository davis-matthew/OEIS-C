/*
 * A115944  Number of partitions of n into distinct factorials.
 * https://oeis.org/A115944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n can be expressed as a sum of distinct factorials
 *            (i.e. its factorial number system representation has all digits
 *            0 or 1), otherwise 0. The sequence satisfies a(0)=1 and for n>0,
 *            a(n)∈{0,1}
 * Range:     u8 n <= 285, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool type, if needed */

seq_t a(idx_t n) {
    if (n == 0) return 1;                 /* a(0) = 1 */

    /* Pre‑compute factorials 1!, 2!, … that are <= n.
       We need at most 20 factorials for 64‑bit n. */
    seq_t f[21];                          /* f[1] = 1! , f[2] = 2! , … */
    f[1] = 1;
    idx_t last = 1;                       /* index of largest stored factorial */

    for (idx_t i = 2; ; ++i) {
        seq_t next = f[last] * i;         /* (last!) * i = i!  */
        if (next > n || next < f[last])   /* overflow or exceeds n */
            break;
        ++last;
        f[last] = next;                    /* store ith factorial */
    }

    /* Check factorial representation of n.  Digits must be 0 or 1. */
    seq_t remainder = n;
    for (idx_t t = last; t > 0; --t) {
        seq_t digit = remainder / f[t];
        if (digit > 1)
            return 0;                     /* digit 2 or more → impossible */
        remainder -= digit * f[t];
    }
    return 1; /* all digits are 0 or 1, and remainder becomes 0 */
}
