/*
 * A095843  Triangle read by rows: T(n,k) = (n-k)^((n-k+1)^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (m-p)^((m-p+1)^2) where m = ceil((sqrt(8n+1)-1)/2) (row
 *            number) p = n - m(m-1)/2 (column number, 1‑based)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* End of formula description */

#include <math.h>

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U) result *= base;
        exp >>= 1U;
        if (exp) base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* find the row m containing the n‑th term */
    uint64_t N = (uint64_t)n;            /* safe cast to 64‑bit for math */
    uint64_t m = (uint64_t)((sqrt(8.0 * N + 1.0) - 1.0) / 2.0 + 1.0);

    /* correct m if the approximation overshot */
    while (m * (m - 1U) / 2U >= N) --m;
    while (m * (m + 1U) / 2U < N) ++m;

    uint64_t prev = m * (m - 1U) / 2U;    /* last index of previous row */
    uint64_t pos  = N - prev;             /* position within the row (1‑based) */

    /* compute the value (m-pos)^((m-pos+1)^2) */
    uint64_t base = m - pos;              /* base: n-k in the original formula */
    if (base == 0U) return 0;             /* base 0 -> value 0 */

    uint64_t base_plus = base + 1U;
    idx_t    exp  = base_plus * base_plus;   /* exponent fits in idx_t */

    return pow_seq_t((seq_t)base, exp);
}
