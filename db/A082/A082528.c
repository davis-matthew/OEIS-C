/*
 * A082528  Least k such that x(k)=0 where x(1)=n x(k)=k^3*floor(x(k-1)/k^3).
 * https://oeis.org/A082528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = least k such that after k iterations of
 *   x_0 := n,
 *   x_i := i³ · ⌊ x_{i-1} / i³ ⌋
 *   the value x_k equals 0.
 * The function iteratively applies the recurrence until the
 * intermediate value becomes zero and returns the current k.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;   /* current value of the sequence */
    for (idx_t k = 1; ; ++k) {
        /* compute k³, careful with potential overflow in the
           unsigned semantics – overflow makes k³ larger than x
           anyway, so y will be zero and we return k. */
        seq_t cube = (seq_t)k * k * k;
        seq_t y = x / cube;       /* integer division, truncates */
        if (y == 0)
            return k;              /* x becomes 0 at this step */
        x = cube * y;             /* update x for next iteration */
    }
}
