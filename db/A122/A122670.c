/*
 * A122670  If n mod 4 = 2 or n mod 4 = 3 then a(n) = 0 else let m=floor(n/4),
 *          then a(n) = (2*m)!/m!.
 * https://oeis.org/A122670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*⌊n/4⌋)! / (⌊n/4⌋)! if n % 4 == 0 or 1, a(n) = 0
 *            otherwise. Here (k)! denotes the factorial of k, and ⌊x⌋ is the
 *            integer floor
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 35, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is 2 or 3 mod 4, the value is zero. */
    if ((n & 3) == 2 || (n & 3) == 3)
        return 0;

    /* Compute m = floor(n/4). */
    idx_t m = n >> 2;           /* same as n / 4 for unsigned n */

    /* The ratio (2*m)! / m! equals ∏_{k=m+1}^{2*m} k. */
    seq_t result = 1;
    if (m > 0) {
        idx_t start = m + 1;
        idx_t end   = 2 * m;
        for (idx_t i = start; i <= end; ++i)
            result *= (seq_t)i;
    }
    /* For m == 0, the product is empty and result remains 1. */
    return result;
}
