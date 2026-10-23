/*
 * A100550  a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3), for n>3, otherwise a(n) = n.
 * https://oeis.org/A100550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) Initial values: a(0)=0,
 *            a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly */
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;

    /* Use a sliding window of size three to compute the recurrence */
    seq_t a1 = 1;   /* a(n-3) at start: a(1) */
    seq_t a2 = 2;   /* a(n-2) at start: a(2) */
    seq_t a3 = 4;   /* a(n-1) at start: a(3) */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = a3 + 2*a2 + 3*a1 */
        next = a3 + (seq_t)2 * a2 + (seq_t)3 * a1;

        /* slide the window */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }

    return a3;
}
