/*
 * A159677  Expansion of 64*x^2/(1 - 1023*x + 1023*x^2 - x^3).
 * https://oeis.org/A159677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=64, a(n)=1023*a(n-1)-1023*a(n-2)+a(n-3)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)64;

    /* Initialize the three previous terms:
       a0 = a(n-3), a1 = a(n-2), a2 = a(n-1) */
    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)64;

    /* Iterate the linear recurrence up to the desired index */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)1023 * a2 - (seq_t)1023 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
