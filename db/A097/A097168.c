/*
 * A097168  Expansion of (1-7x)/((1-x)(1-9x)(1-10x)).
 * https://oeis.org/A097168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*a(n-1) - 109*a(n-2) + 90*a(n-3) with initial values
 *            a(0)=1, a(1)=13, a(2)=151
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)151;

    /* Iteratively apply the recurrence */
    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)13;  /* a(n-2) */
    seq_t a2 = (seq_t)151; /* a(n-1) */
    idx_t i;

    for (i = 3; i <= n; i++) {
        /* next = 20*a2 - 109*a1 + 90*a0 */
        /* The arithmetic is performed using seq_t, so overflow
           automatically wraps for the 64‑bit build; the results are
           correct for all n that fit into the seq_t type. */
        seq_t next = (seq_t)(20 * a2) - (seq_t)(109 * a1) + (seq_t)(90 * a0);

        /* Shift the window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;
}
