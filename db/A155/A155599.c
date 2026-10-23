/*
 * A155599  a(n) = 8^n - 2^n + 1^n.
 * https://oeis.org/A155599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n - 2^n + 1^n Recurrence used: a(0)=1, a(1)=7,
 *            a(n)=10*a(n-1)-16*a(n-2)+7 for n>=2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)7;   /* a(1) */
    seq_t ai  = a1;        /* current value */

    /* iterate from i=2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        ai = 10 * a1 - 16 * a0 + 7;
        a0 = a1;
        a1 = ai;
    }
    return ai;
}
