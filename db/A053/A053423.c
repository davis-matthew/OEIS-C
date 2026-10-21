/*
 * A053423  The mapping F(n) = floor(floor(n*sqrt(n)) / floor(sqrt(n)))
 *          iterates to a fixed point; a(n) is the least k such that
 *          f(k,n)=f(k-1,n), where f(0,n)=n, f(1,n)=F(n), f(k,n)=f(k-1,F(n)).
 * https://oeis.org/A053423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal k ≥ 1 such that F^k(n) = F^{k−1}(n), * where F(t)
 *            = floor_sqrt(t³) / floor_sqrt(t). The function * computes the
 *            number of iterations needed for the mapping * F to reach its
 *            fixed point
 * Range:     u8 n <= 2, u16 n <= 20, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t cur = n;      /* current value of the iteration */
    seq_t count = 0;    /* number of steps taken */

    while (1) {
        seq_t sqrt_cur    = floor_sqrt(cur);      /* floor √cur   */
        seq_t cur_cubed   = cur * cur * cur;      /* cur³          */
        seq_t sqrt_cubed  = floor_sqrt(cur_cubed);/* floor √(cur³) */
        seq_t next        = sqrt_cubed / sqrt_cur;/* F(cur)       */

        count++;                /* we performed one iteration   */
        if (next == cur)        /* fixed point reached          */
            break;

        cur = next;             /* continue iteration           */
    }

    return count;
}
