/*
 * A066007  a(n) is that n-digit number m which minimizes m/(sum of digits of
 *          m); in case of a tie pick the smallest.
 * https://oeis.org/A066007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   zeros_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066007 – minimum n‑digit number m with minimal m / (sum of digits of m)
 *  The sequence follows a closed form:
 *    For n = 1,2,3 : a(1)=1, a(2)=19, a(3)=199
 *    For n ≥ 4 : a(n) = 10^(n-1) + 10^(n-1-r) – 1
 *  where r = the largest integer ≥1 such that
 *          n ≥ (10^r – 1)/9 + r + 2
 *  (i.e. r is the number of consecutive zeros after the leading 1).
 *  The code implements this formula directly using seq_t arithmetic.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/zeros_count.h"

/* helper: power of 10 in seq_t */
static seq_t pow10_seq_t(idx_t exp)
{
    seq_t v = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        v *= (seq_t)10;
    return v;
}

seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)1;
    if (n == 2U)
        return (seq_t)19;
    if (n == 3U)
        return (seq_t)199;

    /* n >= 4 */
    idx_t r = zeros_count(n);

    /* compute 10^(n-1) and 10^(n-1-r) */
    seq_t p1 = pow10_seq_t(n - 1);
    seq_t p2 = pow10_seq_t(n - 1 - r);

    /* a(n) = 10^(n-1) + 10^(n-1-r) – 1 */
    return p1 + p2 - (seq_t)1;
}
