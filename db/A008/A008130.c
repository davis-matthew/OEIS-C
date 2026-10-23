/*
 * A008130  a(n) = floor(n/3)*ceiling(n/3).
 * https://oeis.org/A008130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/3)*ceiling(n/3)
 * Range:     u8 n <= 47, u16 n <= 767, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor division */
    seq_t k = (seq_t)n / 3;          /* k = floor(n/3) */
    seq_t res = k * k;              /* result starts as k*k */

    /* Add k if n is not a multiple of 3 (ceil gives k+1) */
    if (n % 3)
        res += k;                   /* k*(k+1) = k*k + k */

    return res;
}
