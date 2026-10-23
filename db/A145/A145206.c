/*
 * A145206  Numbers x such that 201*x^2 + 67 is a square.
 * https://oeis.org/A145206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=293, a(2)=301845963, a(n+2)=1030190*a(n+1)-a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return (seq_t)293;
    if (n == 2) return (seq_t)301845963;

    seq_t prev2 = (seq_t)293;          /* a(n-2) */
    seq_t prev1 = (seq_t)301845963;    /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)1030190 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
