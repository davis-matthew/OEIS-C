/*
 * A146883  a(n) = 6 * Sum_{m=0..n} 5^m.
 * https://oeis.org/A146883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 6;  a(n) = 5*a(n-1) + 6  for n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 6;
    }

    seq_t result = 6;                    /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        result = result * 5 + 6;         /* a(i) from a(i-1) */
    }
    return result;
}
