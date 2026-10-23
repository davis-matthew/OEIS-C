/*
 * A166139  Triangle T(n,k) read by rows. A080305(A126988(n,k)) if k|n, 0
 *          otherwise.
 * https://oeis.org/A166139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle T(r,k) read by rows, where * T(r,k) =
 *            A080305(r/k) if k | r, otherwise 0. * A080305(m) = m when m = 1
 *            or m is prime, otherwise 1
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple prime test for idx_t values <= 2^64. */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x % 2ULL == 0ULL) return x == 2ULL;
    for (idx_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Return the row number r such that r*(r+1)/2 >= n.  n ≥ 1. */
static idx_t row_of(idx_t n)
{
    idx_t r = 1;
    idx_t tri = 1;          /* r*(r+1)/2 for current r */
    while (tri < n) {
        ++r;
        tri += r;           /* tri now equals r*(r+1)/2 */
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for out‑of‑range call */

    idx_t r = row_of(n);                     /* row index */
    idx_t prev = (r - 1) * r / 2;             /* first index of this row */
    idx_t k = n - prev;                       /* column index (1‑based) */

    if (r % k != 0)                          /* k must divide r */
        return (seq_t)0;

    idx_t m = r / k;                          /* argument for A080305 */

    if (m <= 1)
        return (seq_t)1;
    return is_prime(m) ? (seq_t)m : (seq_t)1;
}
