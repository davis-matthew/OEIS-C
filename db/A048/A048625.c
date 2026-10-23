/*
 * A048625  Pisot sequence P(4,6).
 * https://oeis.org/A048625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) for n ≥ 3; initial values: a(0)=4,
 *            a(1)=6, a(2)=9
 * Range:     u8 n <= 10, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 228, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the nth term of A048625 (Pisot sequence P(4,6)) */
seq_t a(idx_t n)
{
    /* Handle the first three terms */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)9;

    /* a_m3 = a(n-3), a_m2 = a(n-2), a_m1 = a(n-1) */
    seq_t a_m3 = (seq_t)4;   /* a(0) */
    seq_t a_m2 = (seq_t)6;   /* a(1) */
    seq_t a_m1 = (seq_t)9;   /* a(2) */

    /* Compute terms up to n */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = a_m1 + a_m3;   /* a(i) = a(i-1) + a(i-3) */
        a_m3 = a_m2;                /* shift window */
        a_m2 = a_m1;
        a_m1 = next;
    }

    return a_m1;
}
