/*
 * A197189  a(n) = 3*a(n-1) + 5*a(n-2), with a(0)=1, a(1)=2.
 * https://oeis.org/A197189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + 5*a(n-2),  a(0) = 1,  a(1) = 2 */
seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Iteratively compute using the recurrence */
    seq_t prev2 = (seq_t)1; /* a(0) */
    seq_t prev1 = (seq_t)2; /* a(1) */
    seq_t current = (seq_t)0; /* placeholder */
    for (idx_t i = 2; i <= n; ++i)
    {
        current = (seq_t)3 * prev1 + (seq_t)5 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return current;
}
