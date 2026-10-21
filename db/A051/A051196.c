/*
 * A051196  T(2n+3,n), array T as in A050186; a count of aperiodic binary
 *          words.
 * https://oeis.org/A051196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n+3, n) if n mod 3 ≠ 0 a(n) = C(2n+3, n) – C((2n+3)/3,
 *            n/3) if n mod 3 = 0 (C is the binomial coefficient)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the pre‑existing function
   seq_t binomial(idx_t a, idx_t b)  from "../utils/binomial.h" */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* compute m = 2*n + 3;  all arithmetic in idx_t to avoid overflow */
    idx_t m = 2 * n + 3;
    seq_t res = binomial(m, n);

    if (n % 3 == 0) {
        /* subtract the contribution of strings with period dividing 3   */
        idx_t m3 = m / 3;   /* (2*n+3)/3 is integral when n%3==0 */
        idx_t n3 = n / 3;
        res -= binomial(m3, n3);
    }
    return res;
}
