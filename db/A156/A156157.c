/*
 * A156157  a(n) = 6*a(n-1)-a(n-2) for n > 2; a(1) = 17, a(2) = 85.
 * https://oeis.org/A156157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1)-a(n-2), a(1)=17, a(2)=85 */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)85;

    /* Compute iteratively for n >= 3. */
    seq_t prev2 = (seq_t)17; /* a(1) */
    seq_t prev1 = (seq_t)85; /* a(2) */
    seq_t cur = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur     = (seq_t)6 * prev1 - prev2;
        prev2   = prev1;
        prev1   = cur;
    }
    return prev1;
}
