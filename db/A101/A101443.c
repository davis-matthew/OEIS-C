/*
 * A101443  Continued fraction expansion of (I_0(1/2)/I_1(1/2)-1)/2 =
 *          1.56185896... (where I_n is the modified Bessel function of the
 *          first kind).
 * https://oeis.org/A101443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n % 3 == 0) ? (2*(n/3)+1) : 1
 */
seq_t a(idx_t n)
{
    if ((n % 3) == 0)
        return 2 * ((seq_t)(n / 3)) + (seq_t)1;
    else
        return (seq_t)1;
}
