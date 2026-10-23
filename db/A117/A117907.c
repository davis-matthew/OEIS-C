/*
 * A117907  Expansion of x + (1-x)^2/(1-x^6).
 * https://oeis.org/A117907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = floor((5*n-1)/3) mod 2 – 3*[(n mod 6)=1], a(0)=1, a(1)=-1
 * Range:     i8 n <= 331, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -1;
    /* Compute floor((5*n-1)/3) */
    seq_t term = ((seq_t)5 * (seq_t)n - 1) / 3;
    /* term mod 2 */
    seq_t res = term & 1;
    /* subtract 3 if n ≡ 1 (mod 6) */
    if (n % 6 == 1) res -= 3;
    return res;
}
