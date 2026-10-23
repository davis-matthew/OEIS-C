/*
 * A015495  a(0)=1, a(1)=7, a(n) = sum_{k=0}^{k=n-1} 7^k a(k).
 * https://oeis.org/A015495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=7, a(2)=50,
   a(n) = a(n-1) * (7^(n-1) + 1)  for n >= 3 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)50;

    seq_t a_prev = (seq_t)50;     /* a(2) */
    seq_t pow7   = (seq_t)49;     /* 7^(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t mult = pow7 + (seq_t)1;
        a_prev = a_prev * mult;
        pow7 = pow7 * (seq_t)7;
    }
    return a_prev;
}
