/*
 * A100679  Floor of cube root of tetrahedral numbers.
 * https://oeis.org/A100679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((n*(n+1)*(n+2)/6)^(1/3))
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the tetrahedral number Tet(n) = n*(n+1)*(n+2)/6 */
    seq_t nn = (seq_t)n;
    seq_t t  = nn * (nn + 1) * (nn + 2) / 6;

    /* integer cube root by binary search */
    seq_t low  = 0;
    seq_t high = nn + 1;          // high⁰³ > t for all n ≥ 0

    while (low + 1 < high)
    {
        seq_t mid = (low + high) / 2;
        seq_t mid3 = mid * mid * mid;
        if (mid3 <= t)
            low = mid;
        else
            high = mid;
    }

    return low;                   // floor(cuberoot(t))
}
