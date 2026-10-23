/*
 * A125370  Number of base 7 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 7*a(n-1) - a(n-2) - 5*a(n-3) for n > 3 with initial
 *            values a(0)=1, a(1)=7, a(2)=47, a(3)=307. This implementation
 *            iterates the recurrence and returns the result
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)47;
    if (n == 3) return (seq_t)307;

    /* Iterate the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)7;   /* a(n-2) */
    seq_t a2 = (seq_t)47;  /* a(n-1) */
    seq_t a3 = (seq_t)307; /* a(n)  */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        an = 7 * a3 - a2 - 5 * a1; /* a(i) = 7*a(i-1) - a(i-2) - 5*a(i-3) */
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
