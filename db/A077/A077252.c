/*
 * A077252  Sum of digits squared minus sum of digits of n.
 * https://oeis.org/A077252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{digits d of n} (d*d - d)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        idx_t digit = n % 10;      /* digit is 0..9 */
        seq_t d = (seq_t)digit;    /* convert to seq_t for arithmetic */
        sum += d * d - d;          /* add d^2 - d to the sum */
        n /= 10;
    }
    return sum;
}
