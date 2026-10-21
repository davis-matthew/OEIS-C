/*
 * A095835  Triangle read by rows: T(n,k) = n^((n-k)^2), n>=1, 1<=k<=n.
 * https://oeis.org/A095835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = n^((n-k)^2) where k = n - (r-1)r/2 and r is the smallest
 *            integer with r(r+1)/2 ≥ n (row number)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for sqrtl */

/* fast exponentiation for seq_t */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but be safe for n = 0 */
        return (seq_t)0;

    /* 1. Find the row r (number of the row) such that
         r(r+1)/2 >= n.  Use the closed‑form estimate and
         correct it by a single increment if necessary. */
    long double disc = 8.0L * (long double)n + 1.0L;          /* 8n+1 */
    long double s = sqrtl(disc);
    idx_t r = (idx_t)((s - 1.0L) * 0.5L);                     /* floor((sqrt(8n+1)-1)/2) */

    /* adjust upward until r(r+1)/2 ≥ n */
    if ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    /* 2. Compute position k in row r (1‑based) */
    idx_t prev = (unsigned __int128)(r - 1) * r / 2;          /* terms before this row */
    idx_t k = n - prev;                                       /* 1 ≤ k ≤ r */

    /* 3. Compute exponent (r - k)^2 */
    idx_t delta = r - k;
    idx_t exp = delta * delta;

    /* 4. Compute r^exp and return */
    return pow_seq_t((seq_t)r, exp);
}
