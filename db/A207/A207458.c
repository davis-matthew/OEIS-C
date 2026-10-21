/*
 * A207458  Number of 7 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 19*a(n-2) + 6*a(n-3) - 36*a(n-4), a(1)=14,
 *            a(2)=196, a(3)=546, a(4)=3850
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: n >= 1 according to OEIS */
    if (n == 0) return 0;            /* undefined, fall back to 0 */

    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)196;
    if (n == 3) return (seq_t)546;
    if (n == 4) return (seq_t)3850;

    /* Use a sliding window of the last four terms */
    seq_t a0 = (seq_t)14;   /* a(n-4) */
    seq_t a1 = (seq_t)196;  /* a(n-3) */
    seq_t a2 = (seq_t)546;  /* a(n-2) */
    seq_t a3 = (seq_t)3850; /* a(n-1) */
    seq_t next;

    for (idx_t i = 5; i <= n; ++i) {
        next = a3 + (seq_t)19 * a2 + (seq_t)6 * a1 - (seq_t)36 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
