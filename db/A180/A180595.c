/*
 * A180595  Digital root of 5n.
 * https://oeis.org/A180595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = digital_root(5*n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Digital root of 5*n: 0 for n=0, otherwise ((5*n) mod 9), with 0 mapped to 9 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t m = n % 9;               /* reduce n modulo 9 to avoid overflow */
    idx_t r = (5 * m) % 9;          /* compute (5*n) modulo 9 fast */

    return (r == 0) ? (seq_t)9 : (seq_t)r;
}
