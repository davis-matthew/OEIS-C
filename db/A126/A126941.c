/*
 * A126941  The sequence b[n] defined in A126939.
 * https://oeis.org/A126941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 a(1) = 2 a(2) = 4 a(3) = 10 a(4) = 36 a(5) = 116 a(n) =
 *            2*a(n-1) + a(n-2) + 7*a(n-3) + 3*a(n-4) + 3*a(n-5) + a(n-6) for
 *            n ≥ 6 (recurrence from the empirical g.f. of A126941)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first six values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)36;
    if (n == 5) return (seq_t)116;

    /* Recurrence requires the previous six terms */
    seq_t prev[6] = {(seq_t)0, (seq_t)2, (seq_t)4,
                     (seq_t)10, (seq_t)36, (seq_t)116};

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = (seq_t)2 * prev[5] + prev[4]
                   + (seq_t)7 * prev[3] + (seq_t)3 * prev[2]
                   + (seq_t)3 * prev[1] + prev[0];

        /* Shift the window: prev[0] becomes prev[1], etc. */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = next;
    }
    return prev[5];
}
