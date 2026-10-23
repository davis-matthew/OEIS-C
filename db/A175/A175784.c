/*
 * A175784  Numerators of k/(10+k)+1 for k = 2*n-1.
 * https://oeis.org/A175784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator((2*n-1)/(2*n+9)+1) Simplified: a(n) = 4*(n+2) /
 *            g, where g = gcd(10, 2*n+9) = 1 or 5 ⇒ g = 5 if n ≡ 3 (mod 5),
 *            else g = 1 Hence: a(n) = 4*(n+2) if n % 5 != 3 = 4*(n+2)/5
 *            (integer) if n % 5 == 3
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = n + 2, type seq_t to avoid overflow in intermediate */
    seq_t t = (seq_t)n + 2;

    /* Check the 5‑divisibility condition */
    if (n % 5 == 3) {
        /* t is divisible by 5, so divide first to keep a smaller result */
        seq_t t_div5 = t / 5;
        return t_div5 * 4;
    } else {
        return t * 4;
    }
}
