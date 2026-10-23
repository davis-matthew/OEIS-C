/*
 * A035492  Position of card 1 after n-th shuffle in Guy's shuffling problem
 *          (A035485).
 * https://oeis.org/A035492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for n>0, a(n) = (2 * a(n-1)) mod (2*n+1)
 * Range:     u8 n <= 139, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t modulus = (seq_t)((seq_t)2 * (seq_t)i + (seq_t)1);
        cur = (cur + cur) % modulus;
    }
    return cur;
}
