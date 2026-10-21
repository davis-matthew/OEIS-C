/*
 * A083823  a(1) = 15; then numbers obtained at every stage of division by 3
 *          in the following process: multiply by 3, reverse the digits,
 *          divide by 3, reverse the digits, multiply by 3, reverse the digit,
 *          divide by 3.
 * https://oeis.org/A083823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 15; for n ≥ 2, a(n) = 11·10^(n‑2) + (n odd ? 4 : 7)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)15;
    }

    seq_t val = (seq_t)11;                 /* start with 11 */
    idx_t exp = n - 2;                     /* how many 10‑multiplications */

    for (idx_t i = 0; i < exp; ++i) {      /* val = 11 · 10^(n-2) */
        val *= (seq_t)10;
    }

    /* add the suffix 4 or 7 depending on parity of n */
    if (n & 1) {
        val += (seq_t)4;
    } else {
        val += (seq_t)7;
    }

    return val;
}
