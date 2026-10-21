/*
 * A074538  a(n) = 2^n + 5^n + 7^n.
 * https://oeis.org/A074538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 5^n + 7^n Recurrence: a(0)=3, a(1)=14, a(2)=78,
 *            a(n)=14*a(n-1)-59*a(n-2)+70*a(n-3) for n>2 (Uses only integer
 *            arithmetic; works for arbitrary-precision seq_t where overflow
 *            is defined as in the build.)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)78;

    /* Recurrence using only three variables to keep memory usage constant */
    seq_t a0 = (seq_t)3;   /* a(n-3) */
    seq_t a1 = (seq_t)14;  /* a(n-2) */
    seq_t a2 = (seq_t)78;  /* a(n-1) */
    seq_t an;

    for (idx_t i = 3; i <= n; ++i) {
        /* a(n) = 14*a(n-1) - 59*a(n-2) + 70*a(n-3) */
        an = (seq_t)14 * a2 - (seq_t)59 * a1 + (seq_t)70 * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
