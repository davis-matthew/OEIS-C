/*
 * A016295  Expansion of 1/((1 - 2*x)*(1 - 5*x)*(1 - 6*x)).
 * https://oeis.org/A016295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13*a(n-1) - 52*a(n-2) + 60*a(n-3) \ with initial values
 *            a(0) = 1, a(1) = 13, a(2) = 117
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)117;

    /* iterate the recurrence from 3 up to n */
    seq_t a0 = (seq_t)1;     /* a(n-3) */
    seq_t a1 = (seq_t)13;    /* a(n-2) */
    seq_t a2 = (seq_t)117;   /* a(n-1) */
    seq_t new_val;

    for (idx_t i = 3; i <= n; ++i)
    {
        new_val = (seq_t)13 * a2 - (seq_t)52 * a1 + (seq_t)60 * a0;
        a0 = a1;
        a1 = a2;
        a2 = new_val;
    }

    return a2;
}
