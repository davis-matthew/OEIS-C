/*
 * A055949  n - reversal of base 4 digits of n (written in base 10).
 * https://oeis.org/A055949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   a(n) = n - reverse_base4(n)
 * Range:     i8 n <= 70, i16 n <= 9934, i32 n <= 9934, i64 n <= 9934,
 *            i128 n <= 9934, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n;
    __int128 rev = 0;              /* reversed value in base‑10 */

    /* compute the base‑4 digits of n, building the reverse */
    while (m != 0) {
        rev = rev * 4 + (m & 3ULL);   /* m % 4 and m / 4 by bit ops */
        m >>= 2;
    }

    /* final value: n - reverse_base4(n), computed in 128‑bit */
    __int128 diff = (__int128)n - rev;
    return (seq_t)diff;               /* truncate to seq_t (may overflow if result itself is out of range) */
}
