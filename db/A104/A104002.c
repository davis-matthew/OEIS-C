/*
 * A104002  Triangle T(n,k) read by rows: number of permutations in S_n
 *          avoiding all k-length patterns that start with 1 except one fixed
 *          pattern and containing it exactly once.
 * https://oeis.org/A104002
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((t*t+3*t+4)/2-n)*(n-(t*(t+1)/2))^((t*t+3*t+4)/2-n-1) for
 *            t=floor((-1+sqrt(8*n-7))/2). Offset 2: a(2) = 1. Implementation
 *            reads the triangle T(r,k) = (r-k+1)*k^(r-k) in row-major order
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 142, u64 n <= 386,
 *            u128 n <= 1091, big any n
 * Helpers:   int_sqrt_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt_idx.h"

/* Fast exponentiation for seq_t values. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 2)                /* undefined for n < 2, return 0 */
        return 0;

    /* zero-based offset within triangle */
    idx_t m = n - 2;

    /* find row r: smallest r with r(r+1)/2 > m */
    idx_t tmp = int_sqrt_idx(8 * m + 1);     /* floor(sqrt(8m+1)) */
    idx_t r   = (tmp + 1) / 2;               /* ceil((sqrt(8m+1)+1)/2) */

    /* column k (1‑based) */
    idx_t prevRows = (r - 1) * r / 2;
    idx_t k = m - prevRows + 1;             /* 1 <= k <= r */

    /* compute T(r,k) = (r-k+1) * k^(r-k) */
    seq_t pow_val = pow_seq((seq_t)k, r - k);
    seq_t mult    = (seq_t)(r - k + 1);
    return pow_val * mult;
}
