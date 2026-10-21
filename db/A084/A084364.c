/*
 * A084364  Define the operations M: multiply by 11, D: divide by 11, R:
 *          reverse digits. Sequence gives trajectory of 19 under action of
 *          M,R,D,R.
 * https://oeis.org/A084364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 19 + 9 * (n mod 9)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n modulo 9 is safe because n is unsigned and small enough for idx_t */
    idx_t r = n % 9U;          /* 0 <= r < 9 */
    seq_t res = (seq_t)19 + (seq_t)9 * (seq_t)r;
    return res;
}
