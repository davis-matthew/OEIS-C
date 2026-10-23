/*
 * A187394  a(n) = floor(s*n), where s = 4 - sqrt(8); complement of A187393.
 * https://oeis.org/A187394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4 - sqrt(8)) * n)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"


seq_t a(idx_t n)
{
    if (n == 0)           /* offset 1; behaviour for n=0 is undefined */
        return 0;

    /* work in seq_t to keep all arithmetic exact */
    const seq_t n_seq = (seq_t)n;
    const seq_t m = n_seq << 1;                    /* m = 2 · n              */
    const seq_t t_floor = floor_sqrt((m * m) * 2); /* floor( m · √2 )       */
    const seq_t ceil_m_sqrt2 = t_floor + 1;        /* ceil( m · √2 )        */
    return (n_seq * 4) - ceil_m_sqrt2;            /* a(n) = 4n – ceil(m√2) */
}
