/*
 * A144597  Christoffel word of slope 3/7.
 * https://oeis.org/A144597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+1)*3/10) - floor(n*3/10) * Equivalent to a(n) =
 *            1 if (3*n) mod 10 >= 7 else 0, producing * the periodic pattern
 *            0,0,0,1,0,0,1,0,0,1,
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 3*n mod 10 depends only on n mod 10; use small arithmetic to avoid overflow */
    idx_t rem = (3 * (n % 10)) % 10;
    return (rem >= 7) ? 1 : 0;
}
