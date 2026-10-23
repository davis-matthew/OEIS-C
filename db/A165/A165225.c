/*
 * A165225  a(0)=1, a(1)=5, a(n) = 10*a(n-1) - 5*a(n-2) for n > 1.
 * https://oeis.org/A165225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10*a(n-1) - 5*a(n-2), a(0)=1, a(1)=5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Iteratively compute the recurrence up to n */
    seq_t a_prev = (seq_t)1;   /* a(0) */
    seq_t a_curr = (seq_t)5;   /* a(1) */
    seq_t a_next;

    for (idx_t i = 2; i <= n; ++i) {
        /* a(i) = 10*a(i-1) - 5*a(i-2) */
        a_next = a_curr * (seq_t)10 - a_prev * (seq_t)5;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
