/*
 * A131851  Real part of the function z(n)=Sum(d(k)*i^k: d as in
 *          n=Sum(d(k)*2^k), i=sqrt(-1)).
 * https://oeis.org/A131851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Re(z(n)) * * The complex numbers z(n) are defined
 *            recursively as * z(0) = (0,0) * z(n) = z(floor(n/2)) * (0,1) +
 *            (n mod 2, 0) * with the complex multiplication (a+bi)*(0+1i) =
 *            (-b)+a i. * The real part a(n) is required. * * This
 *            implementation evaluates z(n) by iterating over the * binary...
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base case     */
    if (n == 0)
        return (seq_t)0;

    /* Determine the position of the most significant 1‑bit */
    idx_t tmp = n;
    idx_t msb = 0;          /* index of most significant bit (0‑based) */
    while ((tmp >>= 1) != 0)
        ++msb;

    /* Current coordinates of z(n) during the iteration */
    seq_t real = 0;
    seq_t imag = 0;

    /* Process bits from msb down to 0 */
    for (int k = (int)msb; k >= 0; --k) {
        seq_t bit = (seq_t)((n >> k) & 1u);
        seq_t new_real = -imag + bit;
        seq_t new_imag = real;
        real = new_real;
        imag = new_imag;
    }
    return real;
}
