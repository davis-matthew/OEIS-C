/*
 * A211231  Row sums of A211230.
 * https://oeis.org/A211231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = (n+1)*3^n, a(2*n+1) = (n+9)*3^(n-1) (OEIS A211231)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

static seq_t pow3_u64(idx_t exp)
/* Compute 3^exp where exp fits in idx_t.  Works for both built‑in integer
   and arbitrary‑precision seq_t implementations. */
{
    seq_t result = 1;
    seq_t base   = 3;
    while (exp) {
        if (exp & 1ULL)
            result *= base;
        if (exp > 1)
            base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
/* Return the n-th term of OEIS A211231.  Indexing starts at 0. */
{
    if (n == 0)
        return 1;          /* a(0) = 1 */

    if ((n & 1ULL) == 0) { /* even n = 2*m */
        idx_t m   = n >> 1ULL;
        seq_t pow = pow3_u64(m);
        return ((seq_t)m + 1) * pow;
    }

    /* odd n = 2*m + 1 */
    if (n == 1)
        return 3;          /* a(1) = 3 */

    idx_t m   = n >> 1ULL;      /* m = (n-1)/2 */
    seq_t pow = pow3_u64(m - 1);
    return ((seq_t)m + 9) * pow;
}
