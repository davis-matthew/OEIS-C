/*
 * A127212  a(n) = 5^n*Lucas(n), where Lucas = A000204.
 * https://oeis.org/A127212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n*Lucas(n) Recurrence: a(1)=5, a(2)=75,
 *            a(n)=5*a(n-1)+25*a(n-2) for n>=3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)75;

    /* iterative computation using the linear recurrence */
    seq_t prev2 = (seq_t)5;  /* a(1) */
    seq_t prev1 = (seq_t)75; /* a(2) */
    seq_t current;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(i) = 5*a(i-1) + 25*a(i-2) */
        current = (seq_t)5 * prev1 + (seq_t)25 * prev2;
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}
