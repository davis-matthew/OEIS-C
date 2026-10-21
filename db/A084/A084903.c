/*
 * A084903  Binomial transform of positive cubes.
 * https://oeis.org/A084903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-3) * (n^3 + 9·n^2 + 18·n + 8) (all operations in
 *            seq_t)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute the cubic polynomial in seq_t */
    seq_t poly = nn * nn * nn
               + (seq_t)9 * nn * nn
               + (seq_t)18 * nn
               + (seq_t)8;

    /* For n < 3 we divide by 2^(3-n); for n ≥ 3 we multiply by 2^(n-3) */
    if (n < 3) {
        unsigned long long shift = 3 - n;               /* shift is always non‑negative */
        return poly >> shift;                          /* integer division by 2^shift */
    } else {
        unsigned long long shift = n - 3;
        return poly << shift;                          /* multiplication by 2^shift */
    }
}
