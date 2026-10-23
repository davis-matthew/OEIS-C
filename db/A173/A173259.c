/*
 * A173259  Period 3: repeat [4, 1, 4].
 * https://oeis.org/A173259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4 if n mod 3 ≠ 2, otherwise a(n) = 1 (equivalently a(n) =
 *            a(n-3) with a(1)=4, a(2)=1, a(3)=4)
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based. The period is 3, pattern [4,1,4] */
    idx_t r = n % 3;
    if (r == 2) {
        return (seq_t)1;
    } else {
        return (seq_t)4;
    }
}
