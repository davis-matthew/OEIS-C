/*
 * A132264  Product{0<=k<=floor(log_12(n)), floor(n/12^k)}, n>=1.
 * https://oeis.org/A132264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Π_{k=0}^{⌊log_12(n)⌋} ⌊n / 12^k⌋ Recursively: a(n) = n *
 *            a( ⌊n / 12⌋ ), with base a(n) = n for 1 ≤ n ≤ 11
 * Range:     u8 n <= 59, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 by definition of the sequence. */
    seq_t result = 1;
    idx_t t = n;
    while (t > 0) {
        result *= (seq_t)t;   /* multiply by ⌊n / 12^k⌋ at each step */
        t /= 12;              /* next k: divide by base 12 */
    }
    return result;
}
