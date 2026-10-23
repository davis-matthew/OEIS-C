/*
 * A024552  a(n) = [ n/{n*sqrt(5)} ], where {x} := x - [ x ].
 * https://oeis.org/A024552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n / { n*sqrt(5) } )
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 24, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* {x} = x - floor(x) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n == 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* t = floor(n*sqrt(5)) = floor(sqrt(5*n*n)) */
    seq_t nn  = (seq_t)n * (seq_t)n;        /* n^2   */
    seq_t val = (seq_t)5 * nn;              /* 5*n^2 */
    seq_t t   = floor_sqrt(val);            /* floor(sqrt(5*n^2)) */

    idx_t low  = 0;
    idx_t high = 10 * n * n;                /* safe upper bound */
    seq_t nseq = (seq_t)n;

    seq_t result = (seq_t)0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);

        seq_t m  = (seq_t)mid;
        seq_t m1 = (seq_t)(mid + 1);

        /* inequality 1 : (n + m*t)^2 >= 5*m^2*n^2 */
        seq_t prod_mid  = nseq + m * t;
        seq_t left1     = prod_mid * prod_mid;
        seq_t right1    = ((((seq_t)5 * m) * m) * nseq) * nseq;

        /* inequality 2 : (n + (m+1)*t)^2 < 5*(m+1)^2*n^2 */
        seq_t prod_m1   = nseq + m1 * t;
        seq_t left2     = prod_m1 * prod_m1;
        seq_t right2    = ((((seq_t)5 * m1) * m1) * nseq) * nseq;

        if (left1 >= right1 && left2 < right2) {
            result = m;
            break;
        } else if (left1 < right1) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return result;
}
