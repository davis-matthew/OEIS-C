/*
 * A221764  Number of n X 3 arrays of occupancy after each element moves to
 *          some horizontal or vertical neighbor, without 2-loops or left
 *          turns.
 * https://oeis.org/A221764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A221764:  Number of n X 3 arrays of occupancy after each element moves
   to some horizontal or vertical neighbour, without 2‑loops or left turns.
   The sequence satisfies the linear recurrence
           a(n) = 8*a(n-1) - 14*a(n-2) + 8*a(n-3) - a(n-4),
   with initial values
           a(1) = 0,  a(2) = 3,  a(3) = 16,  a(4) = 92. */

seq_t a(idx_t n)
{
    /* return 0 for indices below the offset. */
    if (n < 1)
        return (seq_t)0;

    /* handle the first four terms directly. */
    switch (n) {
    case 1: return (seq_t)0;
    case 2: return (seq_t)3;
    case 3: return (seq_t)16;
    case 4: return (seq_t)92;
    default:
        /* rolling recurrence: keep only the last four values. */
        seq_t a1 = (seq_t)0;   /* a(n-4) */
        seq_t a2 = (seq_t)3;   /* a(n-3) */
        seq_t a3 = (seq_t)16;  /* a(n-2) */
        seq_t a4 = (seq_t)92;  /* a(n-1) */
        seq_t an = a4;

        for (idx_t k = 5; k <= n; ++k) {
            an = 8 * a4 - 14 * a3 + 8 * a2 - a1;  /* a(k) */
            a1 = a2;
            a2 = a3;
            a3 = a4;
            a4 = an;
        }
        return an;
    }
}
