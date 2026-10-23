/*
 * A040583  Continued fraction for sqrt(608).
 * https://oeis.org/A040583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A040583 – continued fraction for sqrt(608)
    a(0) = 24
    For n > 0,
        let e be the exponent of 2 in n.
        a(n) = 1          if e = 0 or e = 1
        a(n) = 11         if e = 2
        a(n) = 48         if e ≥ 3
    (All odd prime powers contribute multiplicatively by 1.)
*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)24;

    /* Find exponent of 2 in n */
    idx_t m = n;
    int e = 0;
    while ((m & 1ULL) == 0) {
        e++;
        m >>= 1;
    }

    if (e <= 1)          return (seq_t)1;
    if (e == 2)          return (seq_t)11;
    return (seq_t)48;    /* e >= 3 */
}
