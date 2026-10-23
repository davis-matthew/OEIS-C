/*
 * A123520  Number of vertical dominoes in all possible tilings of a 2n X 3
 *          grid by dominoes.
 * https://oeis.org/A123520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 18*a(n-2) + 8*a(n-3) - a(n-4) Initial values:
 *            a(0)=0, a(1)=4, a(2)=28, a(3)=152, a(4)=744 (derived from the
 *            generating function 4*z*(1-z)/(1-4*z+z^2)^2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: a(1) is first term; a(0) can be returned as 0 */
    if (n == 0) return 0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)28;
    if (n == 3) return (seq_t)152;
    if (n == 4) return (seq_t)744;

    /* Use recurrence a(n) = 8*a(n-1) - 18*a(n-2) + 8*a(n-3) - a(n-4) */
    seq_t a1 = (seq_t)4;   /* a(n-4) */
    seq_t a2 = (seq_t)28;  /* a(n-3) */
    seq_t a3 = (seq_t)152; /* a(n-2) */
    seq_t a4 = (seq_t)744; /* a(n-1) */
    seq_t next = 0;

    for (idx_t i = 5; i <= n; ++i) {
        next = (seq_t)8 * a4 - (seq_t)18 * a3 + (seq_t)8 * a2 - a1;
        /* shift window */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
