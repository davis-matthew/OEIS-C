/*
 * A091629  Product of digits associated with A091628(n). Essentially the same
 *          as A007283.
 * https://oeis.org/A091629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 6 * 2^(n-1) (equivalently 3 * 2^n)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 6;
    for (idx_t i = 1; i < n; ++i) {
        res <<= 1;            /* double the value each step */
    }
    return res;
}
