/*
 * A157116  Numbers k such that k^2 + 1 == 0 (mod 41^2).
 * https://oeis.org/A157116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3362*n - 1681 + 169*(-1)^n)/4
 * Range:     u8 none, u16 n <= 78, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of bounds, safe return */

    /* Compute numerator in 64‑bit (idx_t) to avoid overflow of seq_t */
    idx_t num = 3362 * n;          /* 3362·n */

    if (n & 1)                    /* n odd  → subtract 1850 */
        num -= 1850;
    else                          /* n even → subtract 1512 */
        num -= 1512;

    /* Division by 4 is exact; shift right by 2 bits */
    return (seq_t)(num >> 2);
}
