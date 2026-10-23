/*
 * A024658  n written in fractional base 10/3.
 * https://oeis.org/A024658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = representation of n in fractional base 10/3 as an
   ordinary decimal integer (digits 0–9).  The least–significant digit
   is n mod 10, the next digit is obtained by integer arithmetic:
   x_{k+1} = (3*(x_k – d_k))/10  until x_k becomes 0. */
seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t res = 0;
    seq_t pow10 = 1;
    while (x > 0) {
        seq_t d = x % (seq_t)10;          /* digit 0 <= d < 10 */
        res += d * pow10;                 /* build decimal number */
        pow10 *= (seq_t)10;               /* shift for next digit */
        x = ((x - d) * (seq_t)3) / (seq_t)10;
    }
    return res;
}
