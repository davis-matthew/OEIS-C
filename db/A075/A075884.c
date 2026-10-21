/*
 * A075884  Image of n at the second step of the 3x+1 algorithm.
 * https://oeis.org/A075884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (6*n + (55*n+4)*m - 6*(5*n-2)*m^2 + (5*n-4)*m^3) / 24 *
 *            where m = n mod 4 (m ∈ {0,1,2,3}), gives the image of n at the
 *            second * step of the 3x+1 algorithm
 * Range:     u8 n <= 169, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be ≥ 1; for n == 0 the value is undefined but a zero is
       returned to keep the function well‑defined. */
    if (n == 0) return 0;

    /* m = n mod 4; using bitwise AND as idx_t is unsigned. */
    idx_t m = n & 3;

    /* Use 128‑bit arithmetic to avoid overflow in the intermediate
       computation.  The intermediate result is divisible by 24. */
    __int128 N = n;
    __int128 M = m;
    __int128 num = (__int128)6 * N;
    num += ((__int128)55 * N + 4) * M;
    num -= (__int128)6 * ((__int128)5 * N - 2) * M * M;
    num += ((__int128)5 * N - 4) * M * M * M;

    /* Convert back to the sequence type.  The division is exact. */
    return (seq_t)(num / 24);
}
