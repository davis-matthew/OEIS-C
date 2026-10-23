/*
 * A199420  a(n) = (5*7^n+1)/3.
 * https://oeis.org/A199420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199420
 * Formula: a(n) = (5*7^n + 1) / 3
 * Recurrence: a(0) = 2,  a(n) = 7*a(n-1) - 2   (n ≥ 1)
 */
seq_t a(idx_t n)
{
    seq_t res = 2;                     /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        res = 7 * res - 2;              /* recurrence step */
    return res;
}
