/*
 * A156570  a(n) = 6*a(n-1)-a(n-2) for n > 2; a(1)=17, a(2)=65.
 * https://oeis.org/A156570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(1)=17, a(2)=65
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined – return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Base cases. */
    if (n == 1)
        return (seq_t)17;
    if (n == 2)
        return (seq_t)65;

    /* Iterate the recurrence. */
    seq_t prev2 = (seq_t)17;    /* a(1) */
    seq_t prev1 = (seq_t)65;    /* a(2) */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)6; /* 6*a(i-1) */
        cur -= prev2;           /* - a(i-2)          */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
