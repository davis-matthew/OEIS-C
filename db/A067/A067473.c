/*
 * A067473  Smallest n-digit square starting with 3.
 * https://oeis.org/A067473
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = ceil(sqrt(3*10^(n-1)))^2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   ceil_sqrt_seq, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/ceil_sqrt_seq.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0; /* undefined for n < 2 */
    seq_t p10 = pow10(n - 1);        /* 10^(n-1) */
    seq_t t   = p10 * ((seq_t)3);    /* 3*10^(n-1) */
    seq_t r   = ceil_sqrt_seq(t);    /* ceil(sqrt(t)) */
    return r * r;                    /* square it */
}
