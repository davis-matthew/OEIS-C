/*
 * A092092  Back and Forth Summant S(n, _3): a(n) = Sum_{i=0..floor(2n/3)}
 *          (n-3i).
 * https://oeis.org/A092092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { 0 if n ≡ 0 (mod 3) * 2*(n/3) + 1 if n ≡ 1 (mod 3) *
 *            (n/3) + 1 if n ≡ 2 (mod 3) } * (n is the 1‑based index, offset =
 *            1)
 * Range:     u8 n <= 384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  n/3 gives the k in the piecewise formulas above   */
    idx_t k = n / 3;
    idx_t r = n % 3;

    if (r == 0) {
        return (seq_t)0;
    } else if (r == 1) {
        return (seq_t)( (seq_t)2 * k + 1 );
    } else {            /* r == 2 */
        return (seq_t)( k + 1 );
    }
}
