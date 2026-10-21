/*
 * A055847  a(n) = 49*8^(n-2) if n>=2; a(0)=1, a(1)=6.
 * https://oeis.org/A055847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 6 if n = 1 49 * 8^(n-2) for n >= 2 (The
 *            exponentiation by shifting works for arbitrary‑precision seq_t,
 *            and for a 64‑bit build the shift amount never exceeds 63.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* n >= 2 */
    idx_t shift = 3 * (n - 2);              /* 8^(n‑2) = 2^(3*(n‑2))   */
    return ((seq_t)49) << shift;           /* 49 * 8^(n‑2) */
}
