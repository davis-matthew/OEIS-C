/*
 * A156060  Jacobsthal numbers A001045 mod 9.
 * https://oeis.org/A156060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = J(n) mod 9 (Jacobsthal numbers modulo 9) Period length:
 *            18 J(0)=0, J(1)=1, J(n)=J(n-1)+2*J(n-2) for n≥2 The sequence
 *            repeats every 18 terms
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed values a(0) … a(17) modulo 9 */
    static const seq_t period[18] = {
        0, 1, 1, 3, 5, 2, 3, 7, 4, 0,
        8, 8, 6, 4, 7, 6, 2, 5
    };
    return period[n % 18];
}
