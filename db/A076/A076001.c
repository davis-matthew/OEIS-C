/*
 * A076001  (1/2)*A075998.
 * https://oeis.org/A076001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * ∏_{k=1..n} < n/k > where <x> denotes the integer
 *            that is the second nearest to x (for a rational x = n/k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the second nearest integer to n/k.
   n, k are positive integers. */
static seq_t second_nearest_local(idx_t n, idx_t k)
{
    idx_t q = n / k;      /* floor(n/k)          */
    idx_t r = n % k;      /* remainder of division */

    if (r == 0) {                 /* n/k is integer                */
        return (seq_t)(q + 1);     /* second nearest is next integer */
    }
    if (2 * r < k) {               /* floor is closer, second nearest is ceil */
        return (seq_t)(q + 1);
    }
    /* otherwise floor is closer or tie (2*r == k), second nearest is floor */
    return (seq_t)q;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* not part of the offset, but safe */

    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        prod *= second_nearest_local(n, k);
    }
    return prod / (seq_t)2;
}
