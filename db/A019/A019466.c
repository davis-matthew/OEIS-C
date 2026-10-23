/*
 * A019466  Multiply by 1, add 1, multiply by 2, add 2, etc.; start with 3.
 * https://oeis.org/A019466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3; for n >= 1, if n is odd set a(n) = a(n-1) * ((n+1)/2),
 *            otherwise set a(n) = a(n-1) + (n/2). This implements “multiply
 *            by 1, add 1, multiply by 2, add 2, …; start with 3.”
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 40, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>  /* for size_t on some platforms, not required but harmless */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)3;

    seq_t x = (seq_t)3;
    for (idx_t i = 1; i <= n; ++i) {
        if (i & 1) {                  /* odd i : multiply */
            seq_t k = (((seq_t)i) + 1) / 2;
            x *= k;
        } else {                      /* even i : add */
            seq_t k = ((seq_t)i) / 2;
            x += k;
        }
    }
    return x;
}
