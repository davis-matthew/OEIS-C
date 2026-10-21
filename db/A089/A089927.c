/*
 * A089927  Expansion of 1/((1-x^2)(1-5x+x^2)).
 * https://oeis.org/A089927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-3) + a(n-4) Initial values: a(0) = 1
 *            a(1) = 5 a(2) = 25 a(3) = 120 Recurrence valid for n >= 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return 1;
    if (n == 1) return 5;
    if (n == 2) return 25;
    if (n == 3) return 120;

    /* Use a sliding window of the last four values. */
    seq_t a0 = 1;   /* a(n-4) */
    seq_t a1 = 5;   /* a(n-3) */
    seq_t a2 = 25;  /* a(n-2) */
    seq_t a3 = 120; /* a(n-1) */
    seq_t a_next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 5*a(i-1) - 5*a(i-3) + a(i-4) */
        a_next = 5 * a3 - 5 * a1 + a0;
        /* Shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a_next;
    }
    return a3; /* a(n) */
}
