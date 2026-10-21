/*
 * A068487  Smallest prime equal to n^2 + m^2 with n<m.
 * https://oeis.org/A068487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 174, u32 n <= 872, u64 n <= 872,
 *            u128 n <= 872, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A068487 by searching for the smallest prime of the form
   n^2 + m^2 with m > n. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"  /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* below offset: any value is acceptable */
    idx_t nsq = n * n;            /* use idx_t to avoid overflow of seq_t   */
    for (idx_t m = n + 1; ; ++m) {/* iterate increasing m */
        idx_t msq = m * m;
        idx_t s   = nsq + msq;    /* n^2 + m^2 */
        if (is_prime(s))
            return (seq_t)s;      /* cast to seq_t – exact when fits   */
    }
}
