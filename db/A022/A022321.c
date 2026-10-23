/*
 * A022321  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 1 and a(1) = 7.
 * https://oeis.org/A022321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 1, a(1) = 7
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    /* a_prev2 = a(n-2), a_prev1 = a(n-1) */
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)7;   /* a(1) */
    seq_t a_next  = a_prev2 + a_prev1 + (seq_t)1;   /* a(2) */

    for (idx_t i = 2; i < n; ++i) {
        a_prev2 = a_prev1;
        a_prev1 = a_next;
        a_next  = a_prev2 + a_prev1 + (seq_t)1;
    }

    return a_next;
}
