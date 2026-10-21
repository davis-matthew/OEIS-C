/*
 * A235062  Odd part of n-th superfactorial (A000178).
 * https://oeis.org/A235062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = product_{k=1}^n odd_part(k!)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where odd_part(m) = m with all factors of 2 removed. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t result   = (seq_t)1;   /* a(0) = 1, but we start at n>=1 */
    seq_t odd_fact = (seq_t)1;   /* odd_part(n!) freshly computed below */

    for (idx_t i = 1; i <= n; ++i)
    {
        /* odd part of i : divide out all factors of 2 */
        idx_t tz        = __builtin_ctzll(i);      /* number of trailing zeros in i */
        idx_t odd_val   = i >> tz;                 /* i / 2^tz -- now odd */
        seq_t odd_part  = (seq_t)odd_val;

        /* Update odd_part(n!) */
        odd_fact = seq_mul(odd_fact, odd_part);

        /* Update a(n) = a(n-1) * odd_part(n!) */
        result = seq_mul(result, odd_fact);
    }

    return result;
}
