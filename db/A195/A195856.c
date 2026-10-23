/*
 * A195856  a(n) = T(10,n), array T given by A048505.
 * https://oeis.org/A195856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (n^2+41*n+400)*2^(n-2) - 99
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 7*a(n-1) - 18*a(n-2) + 20*a(n-3) - 8*a(n-4); */
/* Initial values: a(0)=1, a(1)=122, a(2)=387, a(3)=965 */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)122;
    if (n == 2) return (seq_t)387;
    if (n == 3) return (seq_t)965;

    /* Use the recurrence relation */
    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)122; /* a(n-3)   */
    seq_t a2 = (seq_t)387; /* a(n-2)   */
    seq_t a3 = (seq_t)965; /* a(n-1)   */
    seq_t ai;

    for (idx_t k = 4; k <= n; ++k) {
        ai = 7 * a3 - 18 * a2 + 20 * a1 - 8 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
