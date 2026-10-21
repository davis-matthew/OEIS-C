/*
 * A090040  a(n) = (3*6^n + 2^n)/4.
 * https://oeis.org/A090040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (3*6^n + 2^n) / 4
 *          (valid for all n ≥ 0, OEIS A090040)
 */
#include <stddef.h>  /* for NULL if needed; otherwise no extra includes */

seq_t a(idx_t n)
{
    seq_t pow6 = 1;   /* 6^n */
    seq_t pow2 = 1;   /* 2^n */

    /* Compute 6^n and 2^n iteratively.  Both multiplications are
       performed with seq_t, which supports the usual integer
       operators (see build-specific definition of seq_t).  For the
       arbitrary‑precision build this is exact; for an unsigned
       integer build the computation is only performed when the
       resulting value fits in seq_t, which matches the test
       requirements. */
    for (idx_t i = 1; i <= n; ++i) {
        pow6 *= 6;
        pow2 *= 2;
    }

    /* The numerator 3*6^n + 2^n is guaranteed to be divisible by 4,
       so exact integer division is safe.  All operations use
       seq_t, maintaining the same precision as the caller expects. */
    seq_t numerator = pow6 * 3 + pow2;
    return numerator / 4;
}
