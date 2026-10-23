/*
 * A199490  a(n) = (11*7^n + 1)/3.
 * https://oeis.org/A199490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 2, with a(0) = 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A199490: a(n) = (11*7^n + 1)/3 */
/* Returned type: seq_t, index type: idx_t */

seq_t a(idx_t n)
{
    seq_t res = 4;               /* a(0) */
    if (n == 0) return res;

    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * (seq_t)7 - (seq_t)2;  /* a(n) = 7*a(n-1) - 2 */
    }
    return res;
}
