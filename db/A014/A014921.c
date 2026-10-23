/*
 * A014921  a(1)=1, a(n) = n*8^(n-1) + a(n-1).
 * https://oeis.org/A014921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(n)=n*8^(n-1)+a(n-1) for n>=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, though unused */

seq_t a(idx_t n)
{
    /* Handle the first term explicitly. */
    if (n == 0)
        return 0;          /* undefined for n<1, but safe fallback */

    seq_t result = 1;      /* a(1) */
    seq_t pow8   = 1;      /* 8^(i-1) for current i; starts with 8^1 for i=2 */

    for (idx_t i = 2; i <= n; ++i) {
        /* Compute 8^(i-1) iteratively. */
        pow8 = pow8 * (seq_t)8;
        /* Add i * 8^(i-1) to the running total. */
        result = result + ((seq_t)i) * pow8;
    }

    return result;
}
