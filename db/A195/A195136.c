/*
 * A195136  a(n) = ((n+1)^(n-1) + (n-1)^(n-1))/2 for n>=1.
 * https://oeis.org/A195136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^(n-1)+(n-1)^(n-1))/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* n >= 1 by definition of the sequence */
    seq_t n_seq = (seq_t)n;
    seq_t np1 = n_seq + 1;      /* n + 1 */
    seq_t nm1 = n_seq - 1;      /* n - 1 */

    seq_t term1 = ipow(np1, nm1);  /* (n+1)^(n-1) */
    seq_t term2 = ipow(nm1, nm1);  /* (n-1)^(n-1) */

    seq_t sum = term1 + term2;        /* add the two terms */
    return sum / 2;                   /* divide by two (always integer) */
}
