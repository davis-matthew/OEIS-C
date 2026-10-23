/*
 * A129972  a(n) = 2*floor(log_2(n)) + 1.
 * https://oeis.org/A129972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(log_2(n)) + 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * Computes floor(log2(n)) for n >= 1.  The algorithm shifts the
 * most significant bit right until zero, counting shifts.  It
 * works with any unsigned integer type of size up to 64 bits.
 */
static inline idx_t _floor_log2_idx(idx_t n)
{
    /* Assumes n >= 1; the value for n == 0 is undefined but
       harmless for this sequence (never called with n == 0).   */
    idx_t k = 0;
    while (n >>= 1) {
        ++k;
    }
    return k;
}

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1.  For n == 0 we return 0
       as an innocuous placeholder.                            */
    if (n == 0) return (seq_t)0;

    /* Compute floor(log2(n)) */
    idx_t k = _floor_log2_idx(n);

    /* a(n) = 2*k + 1, cast to seq_t to preserve unsigned type. */
    return ((seq_t)2 * (seq_t)k) + (seq_t)1;
}
