/*
 * A138279  Last digit of A136324. After 0, 1, period 4: repeat [1, 2, 5, 6] =
 *          A131800.
 * https://oeis.org/A138279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(0)=0; a(1)=1; for n≥2, a(n) = [1,2,5,6][(n‑2) mod 4]
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    static const seq_t pattern[4] = { (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)6 };
    return pattern[(n - 2) % 4];
}
