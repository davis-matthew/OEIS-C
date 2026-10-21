/*
 * A050140  a(n) = 2*floor(n*phi)-n, where phi = (1+sqrt(5))/2.
 * https://oeis.org/A050140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*floor(n*phi) - n, phi = (1+sqrt(5))/2
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of a non‑negative seq_t value using binary search.
 * Returns floor(sqrt(x)). */
static seq_t _isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t mid, mid2;

    while (low <= high)
    {
        mid = (low + high) / 2;
        mid2 = mid * mid;

        if (mid2 == x)
            return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* offset is 1; n==0 is irrelevant */

    seq_t ns = (seq_t)n;                     /* n as seq_t for arithmetic */

    /* 5 * n * n fits comfortably in seq_t for n up to 10^4  */
    seq_t sq = (seq_t)5 * ns * ns;

    /* floor(n*sqrt(5)) = floor(sqrt(5*n^2)) */
    seq_t sqrt5n = _isqrt_seq(sq);

    /* floor(n*phi) = floor((n + n*sqrt(5)) / 2) */
    seq_t floor_phi_n = (ns + sqrt5n) / 2;

    /* a(n) = 2*floor(n*phi) - n */
    return 2 * floor_phi_n - ns;
}
