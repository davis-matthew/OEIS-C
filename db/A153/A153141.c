/*
 * A153141  Permutation of nonnegative integers: A059893-conjugate of A153151.
 * https://oeis.org/A153141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For n>0, starting with the second most significant bit down to
   the least significant bit, flip each bit. Stop after flipping a bit that
   was originally 1. For n=0 return 0. This implements the permutation
   described in A153141. */
#include <stdint.h>

static inline idx_t msb_pos(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    /* x > 0 when called */
    return 63 - __builtin_clzll((unsigned long long)x);
#else
    idx_t p = 0;
    while (x > 1) { x >>= 1; ++p; }
    return p;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t msb = msb_pos(n);      /* position of the most significant 1 bit */
    seq_t val = (seq_t)n;        /* result, starts as n */

    /* Flip bits from msb-1 down to 0 */
    for (idx_t pos = msb; pos > 0; ) {
        pos--;                   /* next lower bit */
        idx_t mask = (idx_t)1 << pos;
        if (n & mask) {          /* bit was originally 1 */
            val ^= (seq_t)mask;  /* flip it and stop */
            break;
        } else {                 /* bit was 0, flip and continue */
            val ^= (seq_t)mask;
        }
    }
    return val;
}
