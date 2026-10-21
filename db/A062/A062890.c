/*
 * A062890  Number of quadrilaterals that can be formed with perimeter n. In
 *          other words, number of partitions of n into four parts such that
 *          the sum of any three is more than the fourth.
 * https://oeis.org/A062890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n >= 0, a(n) = floor((n^3 + 3n^2 + 12n + 128)/288) if n is
 *            even a(n) = floor((n^3 + 6n^2 - 3n + 128)/288) if n is odd
 *            (Note: for n < 4 the formula gives 0, as required by the OEIS
 *            data.)
 * Range:     u8 n <= 40, u16 n <= 264, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4u)   /* a(0) = a(1) = a(2) = a(3) = 0 */
        return 0;

    unsigned __int128 nn = n;          /* use 128‑bit intermediate to avoid overflow */
    unsigned __int128 num;

    if (n & 1u)   /* odd n */
        num = nn*nn*nn + 6*nn*nn - 3*nn + 128;
    else          /* even n */
        num = nn*nn*nn + 3*nn*nn + 12*nn + 128;

    /* division by 288 truncates toward zero, same as floor for positive values */
    return (seq_t)(num / 288u);
}
