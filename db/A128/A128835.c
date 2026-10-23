/*
 * A128835  Numbers n such that n^n == 2 (mod 7), or 7 divides n^n-2.
 * https://oeis.org/A128835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-6) + 42 for n >= 7, with a(1)=11, a(2)=16, a(3)=32,
 *            a(4)=37, a(5)=38, a(6)=40
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence has period 6 when reduced modulo 42. */
seq_t a(idx_t n)
{
    /* Handle indices less than 1 defensively. */
    if (n == 0U) return 0U;

    static const seq_t offsets[6] = {11, 16, 32, 37, 38, 40};
    idx_t k = (n - 1U) / 6U;   /* cycle count */
    idx_t r = (n - 1U) % 6U;   /* position within the cycle */

    return offsets[r] + ((seq_t)k) * 42U;
}
