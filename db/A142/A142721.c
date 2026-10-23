/*
 * A142721  An even-odd sequence: a(n) = n/2 if n is even, or a(n-1) +
 *          2^floor(log_2(n+1)) otherwise.
 * https://oeis.org/A142721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0;
   a(n)=n/2             if n is even;
   a(n)=a(n-1)+2^⌊log₂(n+1)⌋  if n is odd. */
#include <stddef.h>
#include <stdint.h>

static inline idx_t floor_log2_u64(idx_t v)
{
    /* v > 0 is guaranteed */
    return 63 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    seq_t current = 0;
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            current = 0;
        } else if ((i & 1ULL) == 0) {          /* even */
            current = (seq_t)i / 2;
        } else {                                /* odd */
            idx_t v   = i + 1;
            idx_t lg  = floor_log2_u64(v);
            seq_t pow = (seq_t)1;
            for (idx_t j = 0; j < lg; ++j)
                pow = pow * (seq_t)2;
            current = current + pow;
        }
    }
    return current;
}
